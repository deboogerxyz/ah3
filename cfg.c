#include <cjson/cJSON.h>
#include <stdio.h>    /* sprintf */
#include <stdlib.h>   /* getenv, ... */
#include <sys/stat.h> /* mkdir */ 
#include <pwd.h>      /* getpwuid */
#include <unistd.h>   /* getuid */

#include "cfg.h"

Cfg *cfg;

static const char *
getcfgdir(void)
{
	char *dir;

	dir = getenv("XDG_CONFIG_HOME");
	if (dir)
		return dir;

	char *buf = malloc(sizeof(char) * 256);

	dir = getenv("HOME");
	if (dir) {
		sprintf(buf, "%s/.config", dir);
		return buf;
	}

	struct passwd *pw = getpwuid(getuid());
	if (pw) {
		sprintf(buf, "%s/.config", pw->pw_dir);
		return buf;
	}

	return NULL;
}

void
cfg_load(const char *name)
{
	const char *dir = getcfgdir();

	char *buf = malloc(sizeof(char) * 256);
	sprintf(buf, "%s/ah3/%s", dir, name);

	FILE *f = fopen(buf, "r");
	if (!f)
		return;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	rewind(f);

	size_t read = 0;

	buf = realloc(buf, sizeof(char) * size + 1);
	if (buf)
		read = fread(buf, sizeof(char), size, f);
	fclose(f);
	if (!read)
		return;

	cJSON *json = cJSON_Parse(buf);
	if (!json)
		return;

	{
		cJSON* miscjson = cJSON_GetObjectItem(json, "Misc");

		cJSON* bhop = cJSON_GetObjectItem(miscjson, "Bunny hop");
		if (cJSON_IsBool(bhop))
			cfg->misc.bhop = cJSON_IsTrue(bhop);
	}

	cJSON_Delete(json);
	free(buf);
}

void
cfg_save(const char *name)
{
	cJSON *json = cJSON_CreateObject();

	{
		cJSON* miscjson = cJSON_CreateObject();

		cJSON_AddBoolToObject(miscjson, "Bunny hop", cfg->misc.bhop);

		cJSON_AddItemToObject(json, "Misc", miscjson);
	}

	const char *dir = getcfgdir();

	char *buf = malloc(sizeof(char) * 256);
	sprintf(buf, "%s/ah3", dir);
	mkdir(buf, S_IRWXU | S_IRWXG | S_IRWXO);

	sprintf(buf, "%s/ah3/%s", dir, name);

	FILE *f = fopen(buf, "w");

	if (f) {
		fprintf(f, cJSON_Print(json));
		fclose(f);
	}

	cJSON_Delete(json);
}

void
cfg_init(void)
{
	cfg = calloc(1, sizeof(Cfg));
}

void
cfg_clean(void)
{
	if (cfg)
		free(cfg);
}
