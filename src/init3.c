/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 01:34:08 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/05 01:34:08 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int     init_subshell(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = dup_line(line, subinput);
    if (!subline)
        return (EX_GEN);
    subline->last_exit = lexer_input(subline);
    if (subline->last_exit != 0)
        return (free_line(subline), EX_GEN);
    // print_expr(subline);
    if (subline->exprs)// && subline->last_exit != -1)
        exec_minishell(subline);
    // if (subline->last_exit == -1)
    //     printf("non non non\n");
    if (subline->lexer_err != 0)
        line->lexer_err = -3; // erreur subshell
    line->prev_exit = subline->prev_exit;
    free_line(subline);
    free(subline);
    return (0);
}
