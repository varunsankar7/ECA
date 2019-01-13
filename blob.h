#ifndef BLOB_H
#define BLOB_H

//Basic blob datastructure
typedef struct {
    float* data;
    int w; //width
    int h; //height
    int d; //depth
} BLOB;

//access element of blob
#define blob_data(blob,z,y,x) blob->data[(z)*(blob)->h*(blob)->w + (y)*(blob)->w + (x)]

//number elements in blob
#define blob_size(blob)   ((blob)->w*(blob)->h*(blob)->d)

//number of bytes in blob
#define blob_bytes(blob)  (blob_size(blob)*sizeof(float))

//free blob
#define blob_free(blob)   {free((blob)->data); free(blob);}

//malloc blob with given dimensions (zero init)
BLOB* blob_alloc(int d, int h, int w);

//calloc blob with given dimensions (zero init)
BLOB* blob_calloc(int d, int h, int w);

//duplicate blob
BLOB* blob_duplicate(BLOB* b);

//write blob to text file
void  blob_write_txt(const char* fname, BLOB* b);

//write blob to binary file
void  blob_write_bin(const char* fname, BLOB* b);

//load blob from text file
BLOB* blob_read_txt(const char* fname);

//load blob from binary file
BLOB* blob_read_bin(const char* fname);

#endif
