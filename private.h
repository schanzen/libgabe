/*
	Include glib.h, pbc.h, and gabe.h before including this file.
*/

struct gabe_pub_s
{
	char* pairing_desc;
	pairing_t p;
	element_t g;           /* G_1 */
	element_t h;           /* G_1 */
	element_t gp;          /* G_2 */
	element_t g_hat_alpha; /* G_T */
};

struct gabe_msk_s
{
	element_t beta;    /* Z_r */
	element_t g_alpha; /* G_2 */
};

typedef struct
{
	/* these actually get serialized */
	char* attr;
	element_t d;  /* G_2 */
	element_t dp; /* G_2 */

	/* only used during dec (only by dec_merge) */
	int used;
	element_t z;  /* G_1 */
	element_t zp; /* G_1 */
}
gabe_prv_comp_t;

struct gabe_prv_s
{
	element_t d;   /* G_2 */
	GArray* comps; /* gabe_prv_comp_t's */
};

typedef struct
{
	int deg;
	/* coefficients from [0] x^0 to [deg] x^deg */
	element_t* coef; /* G_T (of length deg + 1) */
}
gabe_polynomial_t;

typedef struct
{
	/* serialized */
	int k;            /* one if leaf, otherwise threshold */
	char* attr;       /* attribute string if leaf, otherwise null */
	element_t c;      /* G_1, only for leaves */
	element_t cp;     /* G_1, only for leaves */
	GPtrArray* children; /* pointers to gabe_policy_t's, len == 0 for leaves */

	/* only used during encryption */
	gabe_polynomial_t* q;

	/* only used during decryption */
	int satisfiable;
	int min_leaves;
	int attri;
	GArray* satl;
}
gabe_policy_t;

struct gabe_cph_s
{
	element_t cs; /* G_T */
	element_t c;  /* G_1 */
	gabe_policy_t* p;
};
