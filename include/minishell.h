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
# include <signal.h>
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
# include <termios.h>
# include <dirent.h> 
# include <errno.h>
# include <stdbool.h>
# include <stdlib.h>

/* ************************************************************************** */
/*                                ENUMS                                       */
/* ************************************************************************** */

typedef enum 	s_token_type
{
	WORD = 0,		// cmd/argument/word générique
	PIPE = 1,			// |
	OR = 2,				// ||
	AND = 3,			// &&
	REDIR_IN = 4,		// <
	REDIR_OUT = 5,		// >
	REDIR_APPEND = 6,	// >>
	HEREDOC = 7,		// <<
	ASSIGNMENT = 8,	// name=value
	NO_OPCTR = 9
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

extern volatile sig_atomic_t g_sig;

typedef struct	s_token
{
	char		*s;
	t_token_type	type;
	t_quoted	quoted;
	int			in_subshell;
	int			in_heredoc;
	int			has_expand;
	int			has_wildcards;
	int			w_expanded;
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
	int			heredoc_fd;
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

typedef struct	s_expr
{
	t_pipeline	*pipeline;
	t_token_type		op_after;
	int			pipe_count;
	int			has_subshell;
	int			has_wildcards;
	struct s_expr	*next;
}				t_expr;

typedef struct	s_cmd
{
	t_redir	*redirect;
	char	**cmd;
	char	*full_path;
	pid_t	id;
	int		status;
	char	**env;
	int		pipe_count;
	int		no_path;
}	t_cmd;

typedef struct	s_save
{
	int		exit;
	char	**envp;
}	t_save;

typedef struct	s_line
{
	char	*input;
	char	*clean;
	size_t	len;
	t_token	*tokens;
	char	**envp;
	char	**path;
	int		last_exit; //$? $$
	int		prev_exit;
	int		lexer_err;
	int		num_expr;
	int		heredoc_flag;
	t_expr	*exprs;
	t_cmd	*cmd;
	struct s_line	*subline;
	t_save	*save;
}		t_line;

/* ************************************************************************** */
/*                               PROTOTYPES                                   */
/* ************************************************************************** */

/* builtin.c */

int	ft_exit(t_cmd cmd, t_line *line);
int	ft_cd(t_cmd cmd, t_line *line);
int	ft_unset(t_cmd cmd, t_line *line);
int	ft_pwd(void);
int	ft_env(t_line *line);
int	ft_echo(t_cmd cmd, t_line *line);
int	is_option_n(char *s);
int	exec_builtin(t_cmd cmd, t_line *line);
int	is_builtin(char *cmd);

/* env.c */

char	**get_path(char **env);

/* error.c */

/* exec.c */

void	exec_minishell(t_line *line);
void	exec_exprs(t_expr *exprs, char **path ,char **env, t_line *line);
pid_t	exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line);
int		get_fd(int *fd_in, int *fd_out, t_redir *redirect, char *cmd);
int	ft_redir(t_redir *redirect, char *cmd);
int	here_doc_content(char *limiter, t_line *line);
t_cmd	get_cmd(t_pipeline pipeline, char **path, char **env);

/* exit.c */

/* free.c */

void	free_line_fork(t_line *line, int i);
void	free_line(t_line *line);
void	free_split(char **s);
void	free_split2(char **s);
void    free_tokens(t_token *tokens);
void    free_exprs(t_expr *exprs);
void    free_pipeline(t_pipeline *pipe);
void    free_cmd_path(t_line *line);

/* init.c */

void    init_minishell(t_line *line, char **envp, int start_flag, t_save *save);
int    init_clean_input(t_line *line);
void    init_line(t_line *line, char **envp, int start_flag, t_save *save);
void	init_token(t_token *token, int multiple_quote, int quote, int i);
t_expr  *init_new_expr(t_line *line, t_token_type op_ctrl);

/* init2.c */

t_assign    *init_pipeline_assign(t_line *line, int i);
t_redir *init_pipeline_redir(t_line *line, int i);
char    **init_pipeline_args(t_line *line, int i);
t_pipeline  init_pipeline(t_line *line, int (*len)[3]);

/* init3.c */

int     init_subshell(t_line *line, t_token *subinput);

/* lexer.c */

int		lexer_input(t_line *line);
int		lexer_token(t_line *line);
int		lexer_split_expr(t_line *line, t_token *temp, t_expr *new, t_expr *expr, int i);
int		lexer_single_expr(t_line *line, t_expr *new, t_expr *expr);

/* lexer2.c */

char    *lexer_input_something(t_line *line, char *s, char *start, char *end);
char    *lexer_simple_char(t_line *line, char *s, char *start, char *end);
char    *lexer_quoted_char(t_line *line, char *s, char *start, char *end);
char    *lexer_subchell_char(t_line *line, char *s, char *start, char *end);


/* lexer3.c */

char    *lexer_last_char(t_line *line, char *s, char *start, char *end);
t_token *token_type(t_token *token);
t_token *has_expand(t_token *token);

/* lexer4.c */
char    *lexer_special_char(t_line *line, char *s, char *start, char *end);
char    *lexer_special_char2(t_line *line, char **s, char **start, char **end);
char    *lexer_special_char3(t_line *line, char **s, char **start, char **end);


/* main.c */

/* parser.c */

t_expr  *parse_new_expr(t_line *line, t_token_type op_ctrl);
t_token *parse_pipeline(t_line *line, t_token *temp, t_expr *new, int (*len)[3], int *i);
int    parse_word(t_line *line, t_expr *new, t_token *temp, int i, int *j);
int    parse_redir(t_line *line, t_expr *new, t_token *temp, int i, int *j);
int    parse_assignment(t_line *line, t_expr *new, t_token *temp, int i, int *j);
/* parser2.c */

void    count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl);
char    *expanded_var(t_line *line, char *var);
void  find_pipe_position(t_expr *new, t_token *temp, int i);
char    *parse_quoted_token(t_line *line, t_token *token);
char	*parse_expand(t_line *line, t_token *token);

/* parser3.c */

char	*new_expanded_content(t_line *line, size_t j, char *s, char *ex_var, size_t old_len);
char	*expanded_content(char *s, t_line *line);

/* print.c */

void	print_token(t_line *line);
void    print_expr(t_line *line);
void    print_error(char *s, t_exit code);

/* signals.c */

void sigint_handler(int sig);
void setup_signals(void);
void setup_signals_child(void);
void sigint_handler_hd(int sig);


/* subshell.c */

char    *new_subinput(t_line *line, t_token *token);
t_line  *dup_line(t_line *line, t_token *subinput);

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
int     is_something(char c);
int		need_expand(char *s);
int		has_wildcards(char *s);
char	**ft_strdup2(char **env);

/* wildcards.c */

t_token	*lst_join(t_token *at, t_token *nw);
t_token	*last_elem_w(t_token *token);
t_token *add_back_w(t_token *old, t_token *new);
int	match_tab(const char **pattern, char c);
int	ft_fnmatch(const char *pattern, const char *str);
t_token     *parse_wildcards(t_line *line, t_token *token);

#endif
