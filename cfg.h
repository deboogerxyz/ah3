#ifndef CFG_H_
#define CFG_H_

typedef struct {
	int bhop;
} MiscCfg;

typedef struct {
	MiscCfg misc;
} Cfg;

void cfg_load(const char *name);
void cfg_save(const char *name);
void cfg_init(void);
void cfg_clean(void);

extern Cfg *cfg;

#endif /* CFG_H_ */
