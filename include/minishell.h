/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:23 by nburgevi          #+#    #+#             */
/*   Updated: 2025/12/03 13:55:10 by nburgevi         ###   ########.fr       */
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

typedef enum e_token_type
{
	WORD = 0,
	PIPE = 1,
	OR = 2,
	AND = 3,
	REDIR_IN = 4,
	REDIR_OUT = 5,
	REDIR_APPEND = 6,
	HEREDOC = 7,
	ASSIGNMENT = 8,
	NO_OPCTR = 9
}		t_token_type;

typedef enum e_quoted
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

extern volatile sig_atomic_t	g_sig;

typedef struct s_token
{
	char			*s;
	t_token_type	type;
	t_quoted		quoted;
	int				in_subshell;
	int				in_heredoc;
	int				has_expand;
	int				has_wildcards;
	int				w_expanded;
	struct s_token	*next;
	struct s_token	*previous;
}		t_token;

typedef struct s_assign
{
	char		*name;
	char		*value;
}				t_assign;

typedef struct s_redir
{
	int			from_fd;
	char		*redir;
	char		*file;
	int			hd_fd;
	int			order;
}				t_redir;

typedef struct s_pipeline
{
	char			**args;
	t_redir			*redirect;
	t_assign		*assign;
	t_pipe			position;
	int				word_count;
	int				redir_count;
	int				assign_count;
}		t_pipeline;

typedef struct s_expr
{
	t_pipeline			*pipeline;
	t_token_type		op_after;
	int					pipe_count;
	int					has_subshell;
	int					has_wildcards;
	struct s_expr		*next;
}				t_expr;

typedef struct s_cmd
{
	t_redir	*redirect;
	char	**cmd;
	char	*full_path;
	pid_t	id;
	int		status;
	int		pipe_count;
	int		no_path;
}	t_cmd;

typedef struct s_save
{
	int		exit;
	char	**envp;
}	t_save;

typedef struct s_line
{
	char			*input;
	char			*clean;
	size_t			len;
	t_token			*tokens;
	char			**envp;
	char			**path;
	int				last_exit;
	int				prev_exit;
	int				lexer_err;
	int				num_expr;
	int				heredoc_flag;
	t_expr			*exprs;
	t_cmd			*cmd;
	struct s_line	*subline;
	t_save			*save;
}		t_line;

typedef struct s_lexer1
{
	int		i;
	t_token	*temp;
	t_expr	*expr;
	t_expr	*new;
	int		flag;
	int		flag_2;
}				t_lexer1;

typedef struct s_expr1
{
	int		i;
	int		len[3];
	t_expr	*new;
	t_token	*temp;
}				t_expr1;

typedef struct s_parse1
{
	char	**split;
	char	**new_args;
	char	**tempo;
	int		i;
	int		word;
	int		m;
	int		l;
}				t_parse1;

typedef struct s_parse2
{
	int	i;
	int	*j;

}				t_parse2;

typedef struct s_sub1
{
	size_t	len;
	char	*subinput;
	size_t	i;
	size_t	j;
	int		subshell;
	int		subshell2;
	char	*temp;
	t_token	*ttemp;
}				t_sub1;

typedef struct s_parse3
{
	char	*s;
	size_t	len_var;
	size_t	len_old_var;
	size_t	len_s;
	size_t	i;
	size_t	j;
	size_t	k;
}				t_parse3;

typedef struct s_parse4
{
	char	*s;
	int		d_quote;
	int		s_quote;
	int		i;
	int		j;
	int		len;
}				t_parse4;

typedef struct s_parse5
{
	int		i;
	int		start;
	int		end;
	int		len;
	int		s_quote;
	int		d_quote;
	char	*var;
}				t_parse5;

typedef struct s_parse6
{
	int		i;
	int		len;
	char	*temp;
	char	*tmp;
}				t_parse6;

typedef struct s_signals
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	new;
	struct termios		t;
	struct termios		old;
}				t_signals;

/* ************************************************************************** */
/*                               PROTOTYPES                                   */
/* ************************************************************************** */

/* builtin.c */

int			exec_builtin(t_cmd cmd, t_line *line, int flag);
int			is_builtin(char *cmd);
int			ft_echo(t_cmd cmd);
int			is_option_n(char *s);
int			ft_env(t_line *line, t_cmd cmd);

/* builtin2.c */

int			ft_cd(t_cmd cmd, t_line *line);
void		update_env_cd(t_line *line, char *oldpwd, char *newpwd);
void		chdir_error_handling(char *path);
int			no_pwd(char *path, char *oldpwd, char *new_env_pwd, t_line *line);
int			ft_cd2(char *path, t_line *line);

/* builtin3.c */

int			ft_export(t_cmd cmd, t_line *line);
char		**export_new_env(t_line *line, int new_size,
				int exist_pos, char *new_var);
int			get_new_env_size(t_line *line, int exist_pos, int flag);
int			replace_env(t_line *line, char **new_env);
int			copy_env_var(char **new_env, char *new_var, int dest);

/* builtin4.c */

int			var_exists(t_line *line, char *name);
void		not_valid_identifier(char *s, int *ex_code);
int			ft_unset(t_cmd cmd, t_line *line);
char		**unset_new_env(t_line *line, int exist_pos,
				char **new_env);
int			ft_pwd(void);

/* builtin5.c */

int			ft_exit(t_cmd cmd, t_line *line, int flag);
long		ft_atol(char *s);
void		exit_no_arg(t_line *line, int flag);
void		exit_numric_arg_required(t_cmd cmd, int flag, t_line *line);
void		exit_with_choosen_code(t_cmd cmd, t_line *line, int flag);

/* env.c */

char		**get_path(char **env);
char		**add_slash(char **tmp);
char		*find_all_path(char **env);

/* exec.c */

void		exec_exprs(t_expr *exprs, char **path, t_line *line);
pid_t		exec_cmd(t_cmd *cmd, int *fd_in, int *fd_out, t_line *line);

/* exec2.c */

t_cmd		get_cmd(t_pipeline pipeline, char **path);
int			get_cmd_full_path(t_cmd *cmd, char **path);
void		free_exec_cmd(t_line *line);

/* exec3.c */

int			get_fd(int *fd_in, int *fd_out, t_redir *redirect, char *cmd);
int			ft_redir(t_redir *redirect, char *cmd);
int			redir_out(t_redir redirect, char *cmd);
int			redir_in(t_redir redirect);

/* exec4.c */

int			hd_c(char *limiter, t_line *line);
int			fill_hd_content(char **res, char *li, t_line *line, int *flag);
void		stop_hd_eof(char *limiter);
int			fill_res(char **res, char **content, t_line *line);

/* free.c */

void		free_line_fork(t_line *line, int i);
void		free_line(t_line *line);
void		free_split(char **s);
void		free_split2(char **s);
void		free_tokens(t_token *tokens);

/* free2.c */

void		free_e(t_expr *exprs);
void		free_p(t_pipeline *pipe);
void		free_redirect(t_pipeline *pipe);
void		free_cmd_path(t_line *line);

/* init.c */

int			init_minishell(t_line *line, char **envp, int start_flag,
				t_save *save);
int			init_clean_input(t_line *line);
void		first_init_line(t_line *line, char **envp);
void		other_init_line(t_line *line, char **envp, t_save *save);
void		init_line(t_line *line, char **envp, int start_flag, t_save *save);

/* init2.c */

t_assign	*init_pipeline_assign(t_line *line, int i);
t_redir		*init_pipeline_redir(t_line *line, int i);
char		**init_pipeline_args(t_line *line, int i);
t_pipeline	init_pipeline(t_line *line, int (*len)[3]);

/* init3.c */

int			init_subshell(t_line *line, t_token *subinput);
t_line		*dup_line(t_line *line, t_token *subinput);

/* init4.c */

void		init_token(t_token *token, int multiple_quote, int quote, int i);
t_expr		*init_new_expr(t_line *line, t_token_type op_ctrl);

/* lexer.c */

int			err_open_heredoc(t_line *line);
int			lex_err(t_line *line);
int			err_mini_parse(t_line *line);
int			lexer_process_char(t_line *line, char **s, char **start,
				char **end);
int			lexer_handle_last_char(t_line *line, char **s, char **start,
				char **end);

/* lexer2.c */

char		*lexer_input_something(t_line *line, char *s, char *start,
				char *end);
char		*lexer_simple_char(t_line *line, char *s, char *start,
				char *end);
char		*lexer_skip_quoted(t_line *line, char *s, int *quote);
char		*lexer_quoted_char(t_line *line, char *s, char *start,
				char *end);
char		*lexer_subchell_char(t_line *line, char *s, char *start,
				char *end);

/* lexer3.c */

char		*lexer_last_char(t_line *line, char *s, char *start, char *end);
t_token		*token_type(t_token *token);
t_token		*has_expand(t_token *token);

/* lexer4.c */

int			add_special_token(t_line *line, char **end);
char		*lexer_special_char4(t_line *line, char **s, char **start,
				char **end);
char		*lexer_special_char(t_line *line, char *s, char *start, char *end);
char		*lexer_special_char2(t_line *line, char **s, char **start,
				char **end);
char		*lexer_special_char3(t_line *line, char **s, char **start,
				char **end);

/* lexer5.c */

int			lexer_finish(t_line *line);
int			lexer_input(t_line *line);
int			l_split_expr(t_line *line, t_lexer1 *lexer);
int			l_single_expr(t_line *line, t_expr *new, t_expr *expr);

/* lexer6.c */

int			l_handle_token(t_line *line, t_lexer1 *lexer);
int			l_handle_subshell(t_line *line, t_lexer1 *lexer);
void		handle_new_subshell_expr(t_lexer1 *lexer, t_line *line);
void		l_init_lexer1(t_lexer1 *lexer, t_line *line);
int			lexer_token(t_line *line);

/* main.c */

/* parser.c */

t_token		*p_pipe(t_line *line, t_expr1 *exp);
int			parse_word(t_line *line, t_expr1 *exp, t_token *temp);
int			parse_redir(t_line *line, t_expr1 *exp, t_token *temp);
int			parse_assignment(t_line *line, t_expr1 *exp, t_token *temp);

/* parser2.c */

void		count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl);
char		*expanded_var(t_line *line, char *var);
void		find_pipe_position(t_expr *new, t_token *temp, int i);
char		*parse_quoted_token(t_line *line, t_token *token);
char		*parse_expand(t_line *line, t_token *token);

/* parser3.c */

char		*first_check_var(char *s, int *len, t_line *line);
char		*expanded_content_helper(char *s, char *new_s, t_line *line);
char		*expanded_content(char *s, t_line *line);

/* parser4.c */

t_expr		*parse_new_expr(t_line *line, t_token_type op_ctrl);
size_t		sft_count_words(const char *s, char c);
void		init_parse1(t_parse1 *p);
void		normalize_spaces(char *s);
int			alloc_new_args(t_expr1 *exp, t_token *temp, t_parse1 *p);

/* parser5.c */

void		free_new_args_failure(t_parse1 *p);
int			dup_split_into_new_args(t_parse1 *p);
int			dup_arg_into_new_args(t_expr1 *exp, t_parse1 *p);
char		**d_w_t(t_line *line, t_expr1 *exp, t_token *temp);
int			fill_new_args(t_expr1 *exp, t_parse1 *p);

/* parser6.c */

int			handle_word(t_line *line, t_expr1 *exp, t_token *temp);
void		handle_subshell(t_line *line, t_expr1 *exp, t_token *temp);
void		handle_redir_or_assign(t_line *line, t_expr1 *exp, t_token *temp);
int			process_token(t_line *line, t_expr1 *exp, t_token *temp);
int			prev_is_redir(t_token *temp);

/* parser7.c */

int			handle_expand_and_quote(t_line *line, t_token *temp);
int			add_word_to_args(t_expr1 *exp, int i, int *j, t_token *temp);
void		set_from_fd(t_expr1 *exp, int i, int j, t_token *temp);
int			handle_heredoc(t_line *line, t_expr1 *exp,
				t_token *temp, t_parse2 *p);
int			set_redir_and_hd(t_line *line, t_expr1 *exp,
				t_token *temp, t_parse2 *p);

/* parser8.c */

int			expand_and_quote_filename(t_line *line, t_token *temp);
int			set_redirect_file(t_line *line, t_expr1 *exp,
				t_token *temp, t_parse2 *p);
int			need_split_expand(t_token *temp);

/* parser9.c */

void		count_token(t_token *temp, int (*len)[3], t_token_type op_ctrl);
void		find_pipe_position(t_expr *new, t_token *temp, int i);
int			init_expand_t(t_parse3 *p, t_parse5 *p2, t_token *token);
void		init_parse4(t_parse4 *p);
int			find_token_length(t_token *token, t_parse4 *p);

/* parser10.c */

char		*parse_multiple_quotes(t_line *line, t_token *token, t_parse4 *p);
char		*parse_quoted_token(t_line *line, t_token *token);
void		init_parse5(t_parse5 *p);
void		init_parse5_loop(t_parse5 *p);
void		define_quotes(t_parse5 *p, t_token *token);

/* parser11.c */

char		*parse__simple_expand(t_parse5 *p, t_token *token);
void		define_len_and_i(t_token *token, t_parse5 *p);
char		*parse_expand_helper(t_parse5 *p, t_line *line, t_token *token);

/* parser12.c */

int			end_of_var(int *i, char *s, char *var);
char		*find_expanded_var(int i, char *s, t_line *line);
char		*replace_var(char *s, int *len, t_line *line);
char		*ft_strjoin_char(char *s, char c);
char		*take_tmp_value(char *new_s, char *tmp);

/* print_err.c */

int			print_err_1(t_token *begin);
int			print_err_2(t_token *begin);
int			print_err_3(t_token *begin);
int			print_err_4(t_token *begin);
int			print_err_5(t_token *begin, t_line *line);

/* signals.c */

void		sigint_handler(int sig);
void		setup_signals(void);
void		setup_signals_child(void);

/* signals2.c */

void		sigint_handler_hd(int sig);
void		sigquit_handler_hd(int sig);
void		sigint_handler_child(int sig);
void		sigquit_handler_child(int sig);

/* signals3.c */

void		handle_signals_child(t_signals *sig);
void		restore_signals_child(t_signals *sig);
void		handle_signals_hd(t_signals *sig);
void		restore_signals_hd(t_signals *sig);

/* subshell.c */

char		*new_subinput(t_line *line, t_token *token);
int			init_sub1(t_sub1 *d, t_token *token);
void		fill_subinput(t_sub1 *d, t_token *token);
void		add_previous_pipes(t_sub1 *d, t_token *token);
void		add_next_pipes(t_sub1 *d, t_token *token);

/* tokenizer.c */

t_token		*create_token(t_line *line, char *s, int len);
t_token		*create_quoted_token(t_line *line, char *s, int len, int quote);
t_token		*last_elem(t_line *line);
void		add_back(t_line *line, t_token *new);
t_token		*has_expand(t_token *token);

/* utils.c */

int			is_quote(int c);
int			is_whitespace(int c);
int			is_special(int c);
int			is_dollar(int c);
int			is_equal(int c);

/* utils2.c */

int			is_subshell(int c);
int			is_assignment(char *s);
char		*dup_assign_name(char *s);
char		*dup_assign_value(char *s);
int			len_whitespace(t_line *line);

/* utils3.c */
int			count_subshell(char *s);
t_quoted	def_quote(int multiple_quote, int quote);
int			is_something(char c);
int			need_expand(char *s);
int			has_wildcards(char *s);

/* utils4.c */

char		**ft_strdup2(char **env);
char		*find_env_var(t_line *line, char *var);
int			ft_isdigit_str(char *s);
int			has_equal(char *s);
void		recup_save(t_line *line, t_save *save);

/* wildcards.c */

int			ft_fnmatch(char *pattern, char *str);
t_token		*parse_wildcards(t_line *line, t_token *token);

/* wildcards2.c */

void		match_tab_advance(char **p, char c, int *ok);
int			match_tab_class(char **pattern, char c, int *neg_out);
int			match_tab(char **pattern, char c);

/* wildcards3.c */

t_token		*lst_join_single(t_token *at, t_token *nw, t_line *line);
t_token		*lst_join_between(t_token *left, t_token *nw, t_token *right);
t_token		*lst_join(t_token *at, t_token *nw, t_line *line);
t_token		*last_elem_w(t_token *token);
t_token		*add_back_w(t_token *old, t_token *new);

#endif
