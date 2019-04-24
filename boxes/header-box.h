/*
 * Copyright (c) 2019 Tibor Bodnar <piratetibi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lessert General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef HEADER_H
#define HEADER_H

typedef struct _HeaderBox HeaderBox;

struct _HeaderBox
{
  Box super;
  wchar_t *title;
  int title_color;
};

HeaderBox *headerbox_new(void);
int headerbox_init(HeaderBox *self, int n_lines, int n_cols, int pos_y, int pos_x);
void headerbox_set_title_color(HeaderBox *self, int color);
int headerbox_set_title(HeaderBox *self, wchar_t *title);

#endif
