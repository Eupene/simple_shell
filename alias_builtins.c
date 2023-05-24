#include "simple_shell.h"

void set_alias(char *var_name, char *value);
int Handle_Alias(char **args, char __attribute__((__unused__)) **front);
void print_alias(alias_t *alias);

/**
 * Handle_Alias - Integrated program which shows all pseudonyms
 *,prints specified aliases with a or changes a pseudonym.
 * @args: a variety of arguments..
 * @front: A double-valued address to args's starting point.
 *
 * Return: If an error occurs -1.
 *         Else - 0.
 */
int Handle_Alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int i, returner = 0;
	char *var;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (returner);
	}
	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		var = _strchr(args[i], '=');
		if (!var)
		{
			while (temp)
			{
				if (_strcmp(args[i], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
			(returner) = create_error(args + i, 1);
		}
		else
			set_alias(args[i], var);
	}
	return (returner);
}

/**
 * set_alias - Sets an alternative value for the alias
 * name that already exists.
 * @var_name: The alias's name.
 * @value: Aalias's value.
 */
void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int len, j, k;
	char *new_value;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}

/**
 * print_alias - The pseudonym in the format name='value' is printed.
 * @alias: Address of an alias.
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}

/**
 * replace_aliases - replaces any matching alias as it goes
 * through the parameters.
 * @args: Arguments are pointed at in 2D.
 *
 * Return: the arguments.
 */
char **replace_aliases(char **args)
{
	alias_t *temp;
	int i;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[i], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[i]);
				args[i] = new_value;
				i--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}
