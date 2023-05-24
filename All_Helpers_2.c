#include "simple_shell.h"

void handle_line(char **line, ssize_t read);
ssize_t get_new_len(char *line);
void logical_ops(char *line, ssize_t *new_len);

/**
 * handle_line - if needed, divides a line that was read from standard input.
 * @line: A pointer to a line read from standard input.
 * @read: The length of line.
 *
 * Description: Separating with a space ";", "||", and "&&".
 *              Replaces "#" with '\0'.
 */
void handle_line(char **line, ssize_t read)
{
	char *old_line, *new_line;
	char previous, current, next;
	size_t m, n;
	ssize_t new_len;

	new_len = get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	n = 0;
	old_line = *line;
	for (m = 0; old_line[m]; m++)
	{
		current = old_line[m];
		next = old_line[m + 1];
		if (m != 0)
		{
			previous = old_line[m - 1];
			if (current == ';')
			{
				if (next == ';' && previous != ' ' && previous != ';')
				{
					new_line[n++] = ' ';
					new_line[n++] = ';';
					continue;
				}
				else if (previous == ';' && next != ' ')
				{
					new_line[n++] = ';';
					new_line[n++] = ' ';
					continue;
				}
				if (previous != ' ')
					new_line[n++] = ' ';
				new_line[n++] = ';';
				if (next != ' ')
					new_line[n++] = ' ';
				continue;
			}
			else if (current == '&')
			{
				if (next == '&' && previous != ' ')
					new_line[n++] = ' ';
				else if (previous == '&' && next != ' ')
				{
					new_line[n++] = '&';
					new_line[n++] = ' ';
					continue;
				}
			}
			else if (current == '|')
			{
				if (next == '|' && previous != ' ')
					new_line[n++]  = ' ';
				else if (previous == '|' && next != ' ')
				{
					new_line[n++] = '|';
					new_line[n++] = ' ';
					continue;
				}
			}
		}
		else if (current == ';')
		{
			if (m != 0 && old_line[m - 1] != ' ')
				new_line[n++] = ' ';
			new_line[n++] = ';';
			if (next != ' ' && next != ';')
				new_line[n++] = ' ';
			continue;
		}
		new_line[n++] = old_line[m];
	}
	new_line[n] = '\0';

	free(*line);
	*line = new_line;
}

/**
 * get_new_len - Obtains the updated length of a divided line.
 *               by ";", "||", "&&&", or "#".
 * @line: Line to check.
 *
 * Return: New length of line.
 *
 * Description: shortens the lines that contain '#' comments with '\0'.
 */

ssize_t get_new_len(char *line)
{
	size_t m;
	ssize_t new_len = 0;
	char current, next;

	for (m = 0; line[m]; m++)
	{
		current = line[m];
		next = line[m + 1];
		if (current == '#')
		{
			if (m == 0 || line[m - 1] == ' ')
			{
				line[m] = '\0';
				break;
			}
		}
		else if (m != 0)
		{
			if (current == ';')
			{
				if (next == ';' && line[m - 1] != ' ' && line[m - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[m - 1] == ';' && next != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[m - 1] != ' ')
					new_len++;
				if (next != ' ')
					new_len++;
			}
			else
				logical_ops(&line[m], &new_len);
		}
		else if (current == ';')
		{
			if (m != 0 && line[m - 1] != ' ')
				new_len++;
			if (next != ' ' && next != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
/**
 * logical_ops - searches for logical operators in a line "||" or "&&".
 * @line: A pointer to the char to check in the line.
 * @new_len: Pointer to new_len in get_new_len function.
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char previous, current, next;

	previous = *(line - 1);
	current = *line;
	next = *(line + 1);

	if (current == '&')
	{
		if (next == '&' && previous != ' ')
			(*new_len)++;
		else if (previous == '&' && next != ' ')
			(*new_len)++;
	}
	else if (current == '|')
	{
		if (next == '|' && previous != ' ')
			(*new_len)++;
		else if (previous == '|' && next != ' ')
			(*new_len)++;
	}
}
