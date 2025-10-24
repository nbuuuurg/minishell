
#include "../include/minishell.h"

t_token	*lst_join(t_token *at, t_token *nw)
{
	t_token	*left;
	t_token	*right;
	t_token	*tail;

	if (!at || !nw)
		return (at);
	left = at->previous;
	right = at->next;
	free(at->s);
	free(at);
	while (nw->previous)
		nw = nw->previous;
	if (left)
		left->next = nw;
	nw->previous = left;
	tail = nw;
	while (tail->next)
		tail = tail->next;
	tail->next = right;
	if (right)
		right->previous = tail;
	if (left)
		nw = left;
	else
		nw = nw;
	while (nw->previous)
		nw = nw->previous;
	return (nw);
}

t_token	*last_elem_w(t_token *token)
{
	t_token	*temp;

	temp = NULL;
	if (token)
	{
		temp  = token;
		while (temp->next)
			temp = temp->next;
	}
	return (temp);
}

t_token *add_back_w(t_token *old, t_token *new)
{
	t_token	*temp;

	if (new)
	{
		if (!old)
		{
			old = new;
			new->next = NULL;
			new->previous = NULL;
		}
		else
		{
			temp = last_elem_w(old);
			temp->next = new;
			new->previous = temp;
			new->next = NULL;
		}
	}
	return (old);
}

t_token     *parse_wildcards(t_line *line, t_token *token)
{
    t_token *temp2 = NULL;
    DIR *dir;
    struct dirent   *entry;

    dir = opendir(".");
    if (!dir)
        return (line->last_exit = 1, NULL);
    while((entry = readdir(dir)) != NULL)
    {
        //if is a match(token->s, entry->d_name)
            t_token *temp = ft_calloc(1, sizeof(t_token));
            if (!temp)
                return (line->last_exit = 1, NULL);
            temp->s = ft_strdup(entry->d_name);
            if (!temp->s)
                return (free(temp), line->last_exit = 1, NULL);
            temp2 = add_back_w(temp2, temp);
        //else on change r
    }
    // pas tout le temps que si y a eu changement
    token = lst_join(token, temp2);
    closedir(dir);
    return (token);
}