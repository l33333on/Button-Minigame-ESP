typedef struct {
    uint8_t io_port;
    mcp23x17_t *io_expander_dev;
    char *name;
} IOExpanderPort;