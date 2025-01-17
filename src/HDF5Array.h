#include <Rdefines.h>
#include "S4Vectors_interface.h"

#include "hdf5.h"

#define ERRMSG_BUF_LENGTH 256

#define PRINT_TO_ERRMSG_BUF(...) \
	snprintf(_HDF5Array_errmsg_buf, ERRMSG_BUF_LENGTH, __VA_ARGS__)

/* A data structure for representing an HDF5 dataset (tailored based on the
   needs of C_h5mread()). */
typedef struct {
	hid_t dset_id, dtype_id, space_id, plist_id, mem_type_id;
	char *storage_mode_attr;
	H5T_class_t H5class;
	size_t size, ans_elt_size, chunk_data_buf_size;
	SEXPTYPE Rtype;
	int ndim, *h5nchunk;
	hsize_t *h5dim, *h5chunk_spacings;
} DSet;

static inline long long int _get_trusted_elt(SEXP x, int i)
{
	return IS_INTEGER(x) ? (long long int) INTEGER(x)[i] :
			       (long long int) REAL(x)[i];
}


/* array_selection.c */

char _HDF5Array_errmsg_buf[ERRMSG_BUF_LENGTH];

int _shallow_check_selection(
	SEXP starts,
	SEXP counts
);

long long int _check_selection(
	SEXP starts,
	SEXP counts,
	const long long int *dim,
	int *selection_dim_buf
);

SEXP C_check_selection(
	SEXP starts,
	SEXP counts,
	SEXP dim
);

long long int _check_ordered_selection(
	SEXP starts,
	SEXP counts,
	const long long int *dim,
	int *selection_dim_buf,
	int *nstart_buf,
	int *nblock_buf,
	long long int *last_block_start_buf
);

SEXP C_check_ordered_selection(
	SEXP starts,
	SEXP counts,
	SEXP dim
);

int _selection_can_be_reduced(
	int ndim,
	const int *nstart,
	const int *nblock
);

SEXP _reduce_selection(
	SEXP starts, SEXP counts,
	const int *selection_dim,
	const int *nblock,
	const long long int *last_block_start
);

SEXP C_reduce_selection(
	SEXP starts,
	SEXP counts,
	SEXP dim
);

int _map_starts_to_chunks(
	SEXP starts,
	const long long int *dim,
	const long long int *chunk_spacings,
	int *nstart_buf,
	IntAEAE *breakpoint_bufs,
	LLongAEAE *chunkidx_bufs
);

SEXP C_map_starts_to_chunks(
	SEXP starts,
	SEXP dim,
	SEXP chunk_spacings
);


/* DSet.c */

hsize_t *_alloc_hsize_t_buf(
	size_t buflength,
	int zeroes,
	const char *what
);

void _close_DSet(DSet *dset);

int _get_DSet(
	hid_t dset_id,
	int as_int,
	int Rtype_only,
	int ndim,
	DSet *dset
);

hid_t _get_file_id(SEXP filepath);

hid_t _get_dset_id(
	hid_t file_id,
	SEXP name,
	SEXP filepath
);

SEXP C_get_h5mread_returned_type(
	SEXP filepath,
	SEXP name,
	SEXP as_integer
);


/* h5mread.c */

SEXP C_h5mread(
	SEXP filepath,
	SEXP name,
	SEXP starts,
	SEXP counts,
	SEXP noreduce,
	SEXP as_integer,
	SEXP method
);

