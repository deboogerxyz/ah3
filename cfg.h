#ifndef CFG_H_
#define CFG_H_

typedef struct {
	int enabled;
	int limit;
} BtCfg;

typedef struct {
	BtCfg bt;
} Cfg;

void cfg_load(const char *name);
void cfg_save(const char *name);
void cfg_delete(const char *name);
void cfg_init(void);
void cfg_clean(void);

extern Cfg *cfg;

#endif /* CFG_H_ */
