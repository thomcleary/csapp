#define INT
#define PROD

#ifdef FLOAT
typedef float data_t;
#define DATA_NAME "Float"
#endif

#ifdef DOUBLE
typedef double data_t;
#define DATA_NAME "Double"
#endif

#ifdef EXTEND
typedef long double data_t;
#define DATA_NAME "Extended"
#endif

#ifdef INT
typedef int data_t;
#define DATA_NAME "Integer"
#endif

#ifdef LONG
/* $begin typedefint */
typedef long data_t;
/* $end typedefint */
#define DATA_NAME "Long"
#endif

#ifdef CHAR
typedef char data_t;
#define DATA_NAME "Char"
#endif

#ifdef PROD
/* $begin operprod */
#define IDENT 1
#define OP *
/* $end operprod */
#define OP_NAME "Product"
#else
#ifdef DIV
#define OP /
#define IDENT 1
#define OP_NAME "Divide"
#else
/* $begin operplus */
#define IDENT 0
#define OP +
/* $end operplus */
#define OP_NAME "Sum"
#endif /* DIV */
#endif /* PROD */

/* $begin adt */
/* Create abstract data type for vector */
typedef struct {
  long len;
  data_t *data;
  /* $end adt */
  long allocated_len; /* NOTE: we don't use this field in the book */
  /* $begin adt */
} vec_rec, *vec_ptr;
/* $end adt */

/* Create vector */
vec_ptr new_vec(long len);

/* Free storage used by vector */
void free_vec(vec_ptr v);

/*
 * Retrieve vector element and store in dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec_ptr v, long index, data_t *dest);

/* Macro version */
#define GET_VEC_ELEMENT(v, index, dest)                                        \
  !((index) < 0 || (index) >= (v)->len) && *(dest) = (v)->data[(index)], 1;

data_t *get_vec_start(vec_ptr v);

/*
 * Set vector element.
 * Return 0 (out of bounds) or 1 (successful)
 */

int set_vec_element(vec_ptr v, long index, data_t val);

/* Get vector length */
long vec_length(vec_ptr v);

/* Set length of vector.  If > allocated length, will reallocate */
void set_vec_length(vec_ptr v, long newlen);
