/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:23 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/25 18:27:42 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                               INCLUDES                                     */
/* ************************************************************************** */

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>

/* ************************************************************************** */
/*                                ENUMS                                       */
/* ************************************************************************** */

typedef enum 	s_token_type
{
	WORD = 0,		// cmd/argument/word générique
	PIPE,			// |
	OR,				// ||
	AND,			// &&
	REDIR_IN,		// <
	REDIR_OUT,		// >
	REDIR_APPEND,	// >>
	HEREDOC,		// <<
	ASSIGNMENT,	// name=value
	NO_OPCTR
} 		t_token_type;

typedef enum	e_quoted
{
	NO_QUOTE = 0,
	SINGLE,
	DOUBLE,
	MULTIPLE
}		t_quoted;

typedef enum e_exit
{
	EX_OK = 0,
	EX_GEN = 1,
	EX_USAGE = 2,
	EX_NOEXEC = 126,
	EX_NOTFOUND = 127,
	EX_SIGINT = 130
}			t_exit;

typedef enum e_pipe
{
	NO_PIPE = 0,
	FIRST,
	MIDDLE,
	LAST
}			t_pipe;

/* ************************************************************************** */
/*                               STRUCTS                                      */
/* ************************************************************************** */

typedef struct	s_token
{
	char		*s;
	t_token_type	type;
	t_quoted	quoted;
	int			in_subshell;
	int			in_heredoc;
	int			has_expand;
	struct s_token	*next;
	struct s_token	*previous;
}		t_token;

typedef struct	s_assign
{
	char		*name;
	char		*value;
}				t_assign;

typedef struct	s_redir
{
	int			from_fd;
	char		*redir;
	char		*file;
	int			order;
}				t_redir;


typedef struct	s_pipeline
{
	char		**args;
	t_redir		*redirect;
	t_assign		*assign;
	t_pipe		position;
	int			word_count;
	int			redir_count;
	int			assign_count;
}		t_pipeline;

typedef struct s_subshell
{
	t_pipeline	*pipeline;
	int			pipe_count;
	t_token_type	op_after;
}		t_subshell;


typedef struct	s_expr
{
	t_subshell	*subshell;
	t_pipeline	*pipeline;
	t_token_type		op_after;
	int			pipe_count;
	struct s_expr	*next;
}				t_expr;

typedef struct	s_line
{
	char	*input;
	char	*clean;
	size_t	len;
	t_token	*tokens;
	char	**envp;
	char	**path;
	int		last_exit; //$? $$
	int		lexer_err;
	int		num_expr;
	t_expr	*exprs;
}		t_line;

typedef struct	s_cmd
{
	int		buildin;
	char	*cmd_buildin;
	t_redir	*redirect;
	char	**cmd;
	char	*full_path;
	pid_t	id;
	int		status;
	char	**env;
}	t_cmd;

/* ************************************************************************** */
/*                               PROTOTYPES                                   */
/* ************************************************************************** */

/* env.c */

char	**get_path(char **env);

/* error.c */

/* exec.c */

void	exec_minishell(t_line *line, char **env);
void	exec_exprs(t_expr *exprs, char **path ,char **env, int *here_doc_fds);
pid_t	exec_cmd(t_cmd cmd, int *fd_in, int *fd_out, int *here_doc_fds);
int		get_fd(int *fd_in, int *fd_out, t_redir *redirect, int *here_doc_fds);
int	ft_redir(t_redir *redirect, int fd_in[2], int fd_out[2], int *here_doc_fds);
int	here_doc_content(char *limiter);
t_cmd	get_cmd(t_pipeline pipeline, char **path, char **env);

/* exit.c */

/* free.c */

void	free_line(t_line *line);
void	free_split(char **s);
void    free_tokens(t_token *tokens);
void    free_exprs(t_expr *exprs);
void    free_pipeline(t_pipeline *pipe);

/* init.c */

void    init_minishell(t_line *line, char **envp);
int    init_clean_input(t_line *line);
void    init_line(t_line *line, char **envp);
void	init_token(t_token *token, int multiple_quote, int quote, int i);
t_expr  *init_new_expr(t_line *line, t_token_type op_ctrl);

/* init2.c */

t_assign    *init_pipeline_assign(t_line *line, int i);
t_redir *init_pipeline_redir(t_line *line, int i);
char    **init_pipeline_args(t_line *line, int i);
t_pipeline  init_pipeline(t_line *line, int (*len)[3]);

/* lexer.c */

int		lexer_input(t_line *line);
int		lexer_token(t_line *line);
int		lexer_subshell_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i);
int		lexer_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i);
int		lexer_single_expr(t_line *line, t_expr *new, t_expr *expr);

/* lexer2.c */

char    *lexer_special_char(t_line *line, char *s, char *start, char *end);
char    *lexer_simple_char(t_line *line, char *s, char *start, char *end);
char    *lexer_quoted_char(t_line *line, char *s, char *start, char *end);
char    *lexer_subchell_char(t_line *line, char *s, char *start, char *end);
char    *lexer_last_char(t_line *line, char *s, char *start, char *end);

/* lexer3.c */

t_token *token_type(t_token *token);
t_token *has_expand(t_token *token);

/* main.c */

/* parser.c */

t_expr  *parse_new_expr(t_line *line, t_token_type op_ctrl);
t_token *parse_pipeline(t_line *line, t_token *temp, t_expr *new, int (*len)[3], int *i);
int    parse_word(t_line *line, t_expr *new, t_token *temp, int i, int *j);
int    parse_redir(t_line *line, t_expr *new, t_token *temp, int i, int *j);
int    parse_assignment(t_line *line, t_expr *new, t_token *temp, int i, int *j);
/* parser2.c */

void    count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl);
void  find_pipe_position(t_expr *new, t_token *temp, int i);
char    *parse_quoted_token(t_line *line, t_token *token);
char	*parse_expand(t_line *line, t_token *token);

/* print.c */

void	print_token(t_line *line);
void    print_expr(t_line *line);
void    print_error(char *s, t_exit code);

/* signals.c */

/* tokenizer.c */

t_token	*create_token(t_line *line, char *s, int len);
t_token	*create_quoted_token(t_line *line, char *s, int len, int quote);
t_token	*last_elem(t_line *line);
void	add_back(t_line *line, t_token *new);
t_token *has_expand(t_token *token);

/* utils.c */

int	is_quote(int c);
int	is_whitespace(int c);
int	is_special(int c);
int	is_dollar(int c);
int	is_equal(int c);

/* utils2.c */

int	is_subshell(int c);
int	is_assignment(char *s);
char	*dup_assign_name(char *s);
char	*dup_assign_value(char *s);
int     len_whitespace(t_line *line);

/* utils3.c */
int count_subshell(char *s);
t_quoted	def_quote(int	multiple_quote, int quote);

#endif
