/*******************************************************************************************************
 * 用getenv() putenv()实现setenv函数和unsetenv函数。
*********************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern char **environ;
int my_setenv(const char *name, const char *value, int overwrite);
int my_unsetenv(const char*string);

int main()
{
	exit(EXIT_SUCCESS);
}

int my_setenv(const char *name, const char *value, int overwrite)
{
	if (name == NULL)
	{
		errno = EINVAL;
		return -1;
	}

	if (override || getenv(name) == NULL)
	{
		size_t len = strlen(name) + strlen(value) + 2;
		char *env_entry = (char *)malloc(len);
		if (env_entry == NULL)
		{
			errno = ENOMEM;
			return -1;
		}
		strcpy(env_entry, name);
		env_entry[strlen(name)] = '=';
		strcpy(&env_entry[strlen(name)+1], value);
		int r = putenv(env_entry);
		return r == 0 ? 0 : -1;
	}
	return 0;
}

int my_unsetenv(const char *name)
{
	if (name == NULL)
	{
		errno = EINVAL;
		return -1;
	}

	size_t n = strlen(name);
	int i = 0, j = 0;
	while (environ[j] != NULL)
	{
		if (strncmp(environ[j], name, n) == 0 && environ[j][0] == '=')
		{
			j++;
		}
		else
		{
			if (j != 1)
			{
				environ[i] = environ[j];
			}
			i++;
			j++;
		}
	}
	environ[i] = NULL;
	return 0;
}