#ifndef CXDS_INTERNAL_H
#define CXDS_INTERNAL_H

static inline void free_data_if(void (*free_data)(void *), void *data) {
    if (free_data) free_data(data);
}

#endif /* CXDS_INTERNAL_H */
