typedef struct {
	void (*debugmsg)(const char *msg, ...);
} Mem;

void mem_init(void);
void mem_clean(void);

extern Mem *mem;
