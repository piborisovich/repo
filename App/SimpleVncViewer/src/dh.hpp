/*
 * GTK VNC Widget, Diffie Hellman
 *
 * Copyright (C) 2008  Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef GTK_VNC_DH_H__
#define GTK_VNC_DH_H__

#include <glib.h>
#include <gcrypt.h>

struct vnc_dh;

struct vnc_dh *vnc_dh_new(gcry_mpi_t prime, gcry_mpi_t generator);

gcry_mpi_t vnc_dh_gen_secret(struct vnc_dh *dh);
gcry_mpi_t vnc_dh_gen_key(struct vnc_dh *dh, gcry_mpi_t inter);
void vnc_dh_free(struct vnc_dh *dh);

void vnc_mpi_to_bytes(const gcry_mpi_t value, guchar* result, size_t size);
gcry_mpi_t vnc_bytes_to_mpi(const guchar* value, size_t size);


#endif
/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 * End:
 */
