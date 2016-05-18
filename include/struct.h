
typedef struct {
    unsigned int timePeriod;
    unsigned int count;
    unsigned int count_old;
} encoder;

typedef struct {
    double speed_reqest;
    double speed_actual;
    unsigned char direction;
    double PID_out;
    encoder enc;
} motor;