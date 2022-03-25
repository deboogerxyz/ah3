typedef struct {
	void (*debugmsg)(const char *msg, ...);
} Mem;

void mem_init(void);
void mem_onunload(void);

extern Mem *mem;
