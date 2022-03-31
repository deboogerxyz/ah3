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
		cJSON* btjson = cJSON_GetObjectItem(json, "Backtrack");

		cJSON* enabled = cJSON_GetObjectItem(btjson, "Enabled");
		if (cJSON_IsBool(enabled))
			cfg->bt.enabled = enabled->valueint;
		cJSON* limit = cJSON_GetObjectItem(btjson, "Time limit");
		if (cJSON_IsNumber(limit))
			cfg->bt.limit = limit->valueint;
	}

	cJSON_Delete(json);
	free(buf);
}

void
cfg_save(const char *name)
{
	cJSON *json = cJSON_CreateObject();

	{
		cJSON* btjson = cJSON_CreateObject();

		cJSON_AddBoolToObject(btjson, "Enabled", cfg->bt.enabled);
		cJSON_AddNumberToObject(btjson, "Time limit", cfg->bt.limit);

		cJSON_AddItemToObject(json, "Backtrack", btjson);
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
cfg_delete(const char *name)
{
	const char *dir = getcfgdir();

	char *buf = malloc(sizeof(char) * 256);
	sprintf(buf, "%s/ah3/%s", dir, name);

	remove(buf);

	free(buf);
}

void
cfg_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Config", NK_MINIMIZED)) {
		static char buf[256];

		nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buf, sizeof(buf), nk_filter_ascii);

		if (nk_button_label(ctx, "Load"))
			cfg_load(buf);

		if (nk_button_label(ctx, "Save"))
			cfg_save(buf);

		if (nk_button_label(ctx, "Delete"))
			cfg_delete(buf);

		nk_tree_pop(ctx);
	}
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
