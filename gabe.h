/*
  Include glib.h and pbc.h before including this file. Note that this
  file should be included at most once.
*/

#if defined (__cplusplus)
extern "C" {
#endif

/*
  A public key.
*/
typedef struct gabe_pub_s gabe_pub_t;

/*
  A master secret key.
*/
typedef struct gabe_msk_s gabe_msk_t;

/*
  A private key.
*/
typedef struct gabe_prv_s gabe_prv_t;

/*
  A ciphertext. Note that this library only handles encrypting a
  single group element, so if you want to encrypt something bigger,
  you will have to use that group element as a symmetric key for
  hybrid encryption (which you do yourself).
*/
typedef struct gabe_cph_s gabe_cph_t;

/*
  Generate a public key and corresponding master secret key, and
  assign the *pub and *msk pointers to them. The space used may be
  later freed by calling gabe_pub_free(*pub) and
  gabe_msk_free(*msk).
*/
void gabe_setup( gabe_pub_t** pub, gabe_msk_t** msk );

/*
  Generate a private key with the given set of attributes. The final
  argument should be a null terminated array of pointers to strings,
  one for each attribute.
*/
gabe_prv_t* gabe_keygen( gabe_pub_t* pub,
                             gabe_msk_t* msk,
                             char** attributes );

/*
  Pick a random group element and encrypt it under the specified
  access policy. The resulting ciphertext is returned and the
  element_t given as an argument (which need not be initialized) is
  set to the random group element.

  After using this function, it is normal to extract the random data
  in m using the pbc functions element_length_in_bytes and
  element_to_bytes and use it as a key for hybrid encryption.

  The policy is specified as a simple string which encodes a postorder
  traversal of threshold tree defining the access policy. As an
  example,

    "foo bar fim 2of3 baf 1of2"

  specifies a policy with two threshold gates and four leaves. It is
  not possible to specify an attribute with whitespace in it (although
  "_" is allowed).

  Numerical attributes and any other fancy stuff are not supported.

  Returns null if an error occured, in which case a description can be
  retrieved by calling gabe_error().
*/
gabe_cph_t* gabe_enc( gabe_pub_t* pub, element_t m, char* policy );

/*
  Decrypt the specified ciphertext using the given private key,
  filling in the provided element m (which need not be initialized)
  with the result.

  Returns true if decryption succeeded, false if this key does not
  satisfy the policy of the ciphertext (in which case m is unaltered).
*/
int gabe_dec( gabe_pub_t* pub, gabe_prv_t* prv,
                gabe_cph_t* cph, element_t m );

/*
  Exactly what it seems.
*/
GByteArray* gabe_pub_serialize( gabe_pub_t* pub );
GByteArray* gabe_msk_serialize( gabe_msk_t* msk );
GByteArray* gabe_prv_serialize( gabe_prv_t* prv );
GByteArray* gabe_cph_serialize( gabe_cph_t* cph );

/*
  Also exactly what it seems. If free is true, the GByteArray passed
  in will be free'd after it is read.
*/
gabe_pub_t* gabe_pub_unserialize( GByteArray* b, int free );
gabe_msk_t* gabe_msk_unserialize( gabe_pub_t* pub, GByteArray* b, int free );
gabe_prv_t* gabe_prv_unserialize( gabe_pub_t* pub, GByteArray* b, int free );
gabe_cph_t* gabe_cph_unserialize( gabe_pub_t* pub, GByteArray* b, int free );

/*
  Again, exactly what it seems.
*/
void gabe_pub_free( gabe_pub_t* pub );
void gabe_msk_free( gabe_msk_t* msk );
void gabe_prv_free( gabe_prv_t* prv );
void gabe_cph_free( gabe_cph_t* cph );

/*
  Return a description of the last error that occured. Call this after
  gabe_enc or gabe_dec returns 0. The returned string does not
  need to be free'd.
*/
char* gabe_error();

#if defined (__cplusplus)
} // extern "C"
#endif
