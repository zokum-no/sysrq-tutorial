#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>

#include <stddef.h>
#include <stdint.h>

# if PTRDIFF_MAX < SIZE_MAX
/* If the following line causes compilation to fail,
 *  * comment it out after taking note of its message.
 *   */
//# error The replace function might invoke undefined \
	behaviour for strings with length greater than \
PTRDIFF_MAX - see comments in the function body
# endif

// https://github.com/millken/c-example/blob/master/replace-1.c
// http://creativeandcritical.net/str-replace-c
char *replace(const char *str, const char *old, const char *new) {
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);

	if (oldlen != newlen) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* this is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	ret = malloc(retlen + 1);

	for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
		/* this is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
	}
	strcpy(r, p);

	return ret;
}

// Code from http://canonical.org/~kragen/strlen-utf8.html
// Written in 2008 by Kragen Javier Sitaker. This paragraph added in 2016.

int strlen_utf8(char *s) {
	int i = 0, j = 0;
	while (s[i]) {
		if ((s[i] & 0xc0) != 0x80) j++;
		i++;
	}
	return j;
}

// From incubot sources, based on vladbot.
char    *get_token(char **src, const char *token_sep)
	/*
	 * Just a little more convenient than strtok()
	 * This function returns a pointer to the first token
	 * in src, and makes src point to the "new string".
	 *
	 */
{
	char    *tok;

	if(!(src && *src && **src))
		return NULL;

	/* first, removes leading token_sep's */
	while(**src && strchr(token_sep, **src))
		(*src)++;

	/* first non token_sep */
	if(**src)
		tok = *src;
	else
		return NULL;

	/* Make *src point after token */
	*src = strpbrk(*src, token_sep);
	if(*src)
	{
		**src = '\0';
		(*src)++;
		while(**src && strchr(token_sep, **src))
			(*src)++;
	}
	else
		*src = "";
	return tok;
}

