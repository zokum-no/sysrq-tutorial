#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>

#include <stddef.h>
#include <stdint.h>

#include "config.h"
#include "helpers.h"

#define GREEN "\e[0;32m"
#define CYAN "\e[0;36m"

#define BRIGHTRED "\e[1;31m"
#define BRIGHTGREEN "\e[1;32m"
#define BRIGHTYELLOW "\e[1;33m"
#define BRIGHTBLUE "\e[1;34m"
#define BRIGHTMAGENTA "\e[1;35m"
#define BRIGHTCYAN "\e[1;36m"
#define BRIGHTWHITE "\e[1;37m"

#define NORMAL "\e[0m"


void stars(int amount) {
	int i;
	printf("%s", BRIGHTGREEN);
	for (i = 0; i != amount; i++) {
		printf("*");
	}
	printf("%s", NORMAL);
}

void version_info(void) {
	printf("tutorial (Valhall Tutorial System 2.1.2) 2.1.2\n");
	printf("Copyright (C) 2016 Kim Roar Foldøy Hauge.\n");
	printf("This is free software; contact author for copying conditions. There is NO\n");
	printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
}

void cleanup(MYSQL *mysql, MYSQL_RES *res, char *s) {
	if (s) {
		free(s);
	}
	
	if (mysql) {
		mysql_free_result(res);
	}

	if (res) {
		mysql_close(mysql);
	}
};


int main(int argc, char *argv[] ) {

	int page;

	// char kapitler[12][36] = { "Velkommen", "Kjøre et program"};


	char query[8000];
	char *w;
	char *s, *s_original;
	int len = 0;
	int wlen;
	int highlight = 0;
	int yellow = 0;
	int dot = 0;
	int comma = 0;
	int newline = 0;
	int header = 1;
	int shown = 0;
	int meny = 0;

	MYSQL *mysql = mysql_init(NULL);
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	char *topic = NULL;

	if (argc == 1) {
		page = 1;
		topic = strdup("noob");
	} else if (argc == 2) {
		topic = strdup("noob");
		if (argv[1]) {
			if (strcmp(argv[1], "--version") == 0) {
				version_info();
				cleanup(mysql, NULL, NULL);
				return 0;
			} else if (strcmp(argv[1], "meny") == 0) {
				topic = strdup("meny");
				page = 1;
				meny = 1;
			} else {
				page = atoi(argv[1]);
			}
			if (!page) {
				topic = strdup(argv[1]);
				page = 0;
			}

		} else {
			page = 1;
		}
	} else if (argc == 3) {
		topic = strdup(argv[1]);
		page = page = atoi(argv[2]);
	} else {
		printf("Error: Too many arguments\n");
	}

	if (!mysql_real_connect(mysql,"localhost","tutorial", PASSWORD,"tutorial",3306,NULL,0)) {
		printf("MySQL error: %s\n", mysql_error(mysql));
		cleanup(mysql, NULL, NULL);
		return 2;
	}

	if (page) {
		sprintf(query,"SELECT title,content FROM topics WHERE number = %d AND topic = '%s'", page, topic);
	} else {
		sprintf(query,"SELECT title,number FROM topics WHERE topic = '%s' ORDER BY number", topic);
	}
	mysql_real_query(mysql,query,(unsigned int)strlen(query));
	res = mysql_use_result(mysql);

	while((row = (mysql_fetch_row(res)))) {
		shown++;
		//stars(79);
		if (header) {
			stars(34);
			printf("%s[ Valhall ]%s", BRIGHTWHITE, NORMAL);
			stars(34);
		}
		if (meny) {
			header = 0;
			printf("\n%s* Våre guider!\n", BRIGHTYELLOW);
			stars(79);
			printf("\n");

		} else if (page) {
			// printf("\n%sValhall: Innføring, kapittel %d: %s\n", BRIGHTYELLOW, page, row[0]);
			/*stars(34);
			printf("%s[ Valhall ]%s", BRIGHTWHITE, NORMAL);
			stars(34);
			*/
			printf("\n%sKapittel %d: %s\n", BRIGHTYELLOW, page, row[0]);
		} else {
			if (header) {
				printf("\n%sInnholdsfortegnelse for '%s'\n", BRIGHTYELLOW, topic);
				stars(79);
				header = 0;
			}
			printf("\n%2s - %s",  row[1], row[0]);
			continue;
		}
		
		if (header) {
			stars(79);
			printf("\n");
			header = 0;
		}
		// printf("%s\n", row[1]);

		s = row[1];

		/*char srch[5];
		sprintf(srch, "%c%c%c%c", '\r', '\n', '\r', '\n');
		char repl[4];
		sprintf(repl, " %c %c ", '\n', '\n');
		*/
		
		char srch[5];
		sprintf(srch, "%c%c", '\r', '\n');
		char repl[4];
		sprintf(repl, " %c ", '\n');

		// s = replace(s, srch, repl);
		s = replace(s, srch, repl);
		s_original = s;

		w = NULL;

		yellow = 0;
		while(w != s) {
			int color;

			wlen = 0;
			dot = 0;
			comma = 0;

			if (s[0] == '\'') {
				color = 1;
			} else if (s[0] == '^') {
				color = 2;
			} else {
				color = 0;
			}

			if (color) {
				s++;
				if (color == 1) {
					printf(BRIGHTYELLOW);
					w = get_token(&s, "'");
				} else {
					printf(BRIGHTMAGENTA);
					w = get_token(&s, "^");
				}
				if (s && s[0] == '.') {
					dot = 1;
				} else if (s && s[0] == ',') {
					comma = 1;
				}
				yellow = 1;
			} else {
				w = get_token(&s, " ");
				if (!w) {
				        w = s;
				}
			}

			wlen = wlen + strlen_utf8(w);

			if (((len + wlen + dot + comma) > 78) && (!strstr(w, "\n")) ) {
				// printf("%d %d |\n", len, wlen);
				printf("\n");
				len = 0;
			}
			if (dot || comma || strstr(w, "\n")) {
				printf("%s", w);
				len = len + wlen;
				newline = 0;
			} else {
				printf("%s ", w);
				len = len + wlen + 1;
			}

			if (yellow) {
				printf(NORMAL);
				yellow = 0;
				if (dot) {
					printf(". ");
					s++;
					len = len + 2;
				} else if (comma) {
					printf(", ");
					s++;
					len = len + 2;
				}
			}
			if (strstr(w, "\n")) {
				len = 0;
				newline = 1;
				// printf("-- \"%s\" --", w);
			}
			// printf("%d", len);

		}
		printf("\n");
		free(s_original);
		mysql_free_result(res);
		mysql_close(mysql);
		free(topic);
		return 0;
	}
	/*free(s_original);
	mysql_free_result(res);
	mysql_close(mysql);*/
	free(topic);
	cleanup(mysql, res, s_original);
	if (page) {
		printf("Error 1: No such chapter (%d) in database.\n", page);
		return 1;
	} else if (shown == 0) {
		printf("Error 2: No such topic (%s) in database.\n", argv[1]);
		return 2;
	} else {
		printf("\n");
	}
	return 0;
}
