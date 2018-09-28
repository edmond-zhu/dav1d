/*
 * Copyright © 2018, VideoLAN and dav1d authors
 * Copyright © 2018, Two Orioles, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dav1d/data.h"

#include "common/validate.h"

#include "src/ref.h"

int dav1d_data_create(Dav1dData *const buf, const size_t sz) {
    validate_input_or_ret(buf != NULL, -EINVAL);

    buf->ref = dav1d_ref_create(sz);
    if (!buf->ref) return -ENOMEM;
    buf->data = buf->ref->data;
    buf->sz = sz;

    return 0;
}

int dav1d_data_wrap(Dav1dData *const buf, uint8_t *const ptr, const size_t sz,
                    void (*free_callback)(uint8_t *data, void *user_data),
                    void *user_data)
{
    validate_input_or_ret(buf != NULL, -EINVAL);
    validate_input_or_ret(ptr != NULL, -EINVAL);
    validate_input_or_ret(free_callback != NULL, -EINVAL);

    buf->ref = dav1d_ref_wrap(ptr, sz, free_callback, user_data);
    if (!buf->ref) return -ENOMEM;
    buf->data = ptr;
    buf->sz = sz;

    return 0;
}

void dav1d_data_unref(Dav1dData *const buf) {
    dav1d_ref_dec(buf->ref);
    memset(buf, 0, sizeof(*buf));
}
