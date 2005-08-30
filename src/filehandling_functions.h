/***************************************************************************
 *  Pinfo is a ncurses based lynx style info documentation browser
 *
 *  Copyright (C) 1999  Przemek Borys <pborys@dione.ids.pl>
 *  Copyright (C) 2005  Bas Zoetekouw <bas@debian.org>
 *  Copyright 2005  Nathanael Nerode <neroden@gcc.gnu.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 ***************************************************************************/

#ifndef __FILEHANDLING_FUNCTIONS_H
#define __FILEHANDLING_FUNCTIONS_H

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <string>

#define INFO_TAG 0x1f
#define INDIRECT_TAG 0x7f

void initpaths ();
void addrawpath (const std::string filename);

/*  seek to a node in certain info file */
void seeknode (int tag_table_pos, FILE ** Id);

/*
 * free allocated memory, hold by buf (node** content, stored line by line),
 * and type (a char* pointer, which stores the node header).
 */
void freeitem (char **type, char ***buf, long *lines);

/*
 * reads a node from 'id' to 'buf', and the header of node to 'type'. It sets
 * the numer of read lines to *lines. Warning! First line of 'buf' is left
 * empty.
 */
void read_item (FILE * id, char **type, char ***buf, long *lines);
/* searches for indirect entry of info file */
int seek_indirect (FILE * id);
/* as above, but with tag table entry */
int seek_tag_table (FILE * id,int quiet);
/*
 * loads indirect table (from a special node, stored in message, of lines
 * length)
 */
void load_indirect (char **message, long lines);
/* loads tag table (as above) */
void load_tag_table (char **message, long lines);
/* opens info file */
FILE *openinfo (const std::string filename, int number);
/* opens dir info file */
FILE *opendirfile (int number);

/* creates tag table for info file */
void create_tag_table (FILE * id);
/* creates tag table for indirect info */
void create_indirect_tag_table ();

/*
 * look up a name, which was specified by the user in cmd line, in dir
 * entries. If found, return filedescriptor of the info file, which holds
 * needed entry. Also set `first node' to the name of node, which describes
 * the problem. Arguments:
 * type: a pointer to char*, which will hold the header line of dir entry
 * message: a pointer to char** buffer, which will hold the dir page line by
 *          line
 * lines: pointer to long, which holds the number of lines in dir entry
 */
FILE *
dirpage_lookup (char **type, char ***message, long *lines,
		const std::string filename, std::string& first_node);

/* removes trailing .gz, .bz2, etc. */
void strip_compression_suffix (std::string& filename);

/* Gets the base file name from a filename-with-directory */
void basename (std::string filename, std::string& basename_str);

#endif
