/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glasset <glasset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 11:17:39 by glasset           #+#    #+#             */
/*   Updated: 2016/10/19 14:33:28 by glasset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
static t_print		*generate_list(int nsyms, int symoff, int stroff, char *ptr, t_flags *flags)
{
	int					i;
	struct nlist_64		*list;
	char				*strtab;
	t_print				*cmd;

	list = (void *) ptr + symoff;
	strtab = ptr + stroff;
	i = 0;
	cmd = new_node(NULL);
	while (i < nsyms)
	{
		if (list[i].n_type & N_STAB)
		{
			i++;
			continue;
		}
		if (list[i].n_value)
			ft_hexa(list[i].n_value, &cmd->hexa, 15);
		else
			cmd->hexa = ft_strdup("                ");
		cmd->type = type(list[i].n_type, list[i].n_sect, list[i].n_value, flags);
		cmd->name = ft_strdup(strtab + list[i++].n_un.n_strx);
		cmd->next = new_node(cmd);
		cmd = cmd->next;
	}
	cmd = cmd->prev;
	cmd->next = NULL;
	return (cmd);
}

static void		handle_segment_64(t_flags *flags, struct segment_command_64 *seg)
{
	int					i;
	struct section_64	*sec;

	i = 0;
	sec = (struct section_64 *) ((char *)seg + sizeof(struct segment_command_64));
	while (i < seg->nsects)
	{
		if (ft_strcmp((sec + i)->sectname, SECT_TEXT) == 0 &&
				ft_strcmp((sec + i)->segname, SEG_TEXT) == 0)
		{
			flags->text_nsect = i + 1;
		}
		else if (ft_strcmp((sec + i)->sectname, SECT_DATA) == 0 &&
				ft_strcmp((sec + i)->segname, SEG_DATA) == 0)
		{
			flags->data_nsect = i + 1;
		}
		else if (ft_strcmp((sec + i)->sectname, SECT_BSS) == 0 &&
				ft_strcmp((sec + i)->segname, SEG_DATA) == 0)
		{
			flags->bss_nsect = i + 1;
		}
		i++;
	}
}

void	header_64(char *ptr)
{
	t_flags					flags;
	struct mach_header_64	*header;
	struct load_command		*command;
	struct symtab_command	*sym;
	unsigned long int		i;
	int						ncmds;

	flags.text_nsect = NO_SECT;
	flags.data_nsect = NO_SECT;
	flags.bss_nsect = NO_SECT;
	i = 0;
	header = (struct mach_header_64 *)ptr;
	command = (void *)ptr + sizeof(*header);
	while (i < header->ncmds)
	{
		if (command->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)command;
			sort(generate_list(sym->nsyms, sym->symoff, sym->stroff, ptr, &flags));
			break;
		}
		if (command->cmd == LC_SEGMENT_64)
		{
			handle_segment_64(&flags, (struct segment_command_64 *)command);
		}
		command = (void *)command + command->cmdsize;
		i++;
	}
}
