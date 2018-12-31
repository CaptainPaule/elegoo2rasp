typedef struct {
	float measure_time;
	unsigned int num;
	float mean;
	float std;
} DATA;

typedef struct {
	int timestamp;
	DATA temp;
} DATAFRAME;
