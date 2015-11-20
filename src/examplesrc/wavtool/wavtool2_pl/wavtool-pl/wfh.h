/*
    wavtool-pl
    Copyright (C) 2011 Ying-Chun Liu (PaulLiu) <paulliu@debian.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _HEADER_WAVFILEHEADER_PAULLIU_
#define _HEADER_WAVFILEHEADER_PAULLIU_

extern int wfh_samplerate;
extern int wfh_channels;
extern int wfh_bits;

void wfh_init(const char *filename);
int wfh_getlength(const char *filename);
int wfh_putlength(const char *filename,int length);


#endif
