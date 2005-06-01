#include "common_includes.h"

RCSID("$Id$")

int verbose = 1;

char *filenameprefix = 0;

char *httpviewer = "lynx";
char *ftpviewer = "lynx";
char *maileditor = "mail";
char *printutility = "lpr";
char *manlinks = "1:8:2:3:4:5:6:7:9:n:l:p:o:3X11:3Xt:3X:3x";
char *configuredinfopath = "/usr/info:/usr/share/info:/usr/local/info";
char *ignoredmacros = 0;
char *rcfile = NULL;

char *tmpfilename1 = 0;
char *tmpfilename2 = 0;

SearchAgain searchagain;

HyperObject *hyperobjects = 0;
int hyperobjectcount = 0;

Indirect *indirect = 0;
TagTable *tag_table = 0;
long FirstNodeOffset = 0;
char FirstNodeName[256];
int IndirectEntries = 0;
int TagTableEntries = 0;
int maxx, maxy;
int CutManHeaders = 0;
int CutEmptyManLines = 0;
int ForceManualTagTable = 0;
int LongManualLinks = 0;
char *ManOptions = "";
char *StderrRedirection = "2> /dev/null";
int FilterB7 = 0;
int ConfirmQuit = 0;
int QuitConfirmDefault = 0;
int ClearScreenAtExit = 0;
int CallReadlineHistory = 1;

InfoHistory infohistory;

int npos = -1;
int ncursor = -1;
int nmenu = -1;
int use_apropos = 0;
int plain_apropos = 0;
int use_manual = 0;
int use_raw_filename = 0;
int quote_ignored = 0;

int winchanged = 0;

void
inithistory()
{
	infohistory.length = 0;
	infohistory.node = 0;
	infohistory.file = 0;
	infohistory.pos = 0;
	infohistory.cursor = 0;
	infohistory.menu = 0;
}

/*
 * Add history entry
 */
void
addinfohistory(char *file, char *node, int cursor, int menu, int pos)
{
	if (!infohistory.length)
	{
		infohistory.length++;
		infohistory.node = xmalloc(sizeof(char *) * 2);
		infohistory.node[0] = 0;
		infohistory.file = xmalloc(sizeof(char *) * 2);
		infohistory.file[0] = 0;
		infohistory.pos = xmalloc(sizeof(int) * 2);
		infohistory.cursor = xmalloc(sizeof(int) * 2);
		infohistory.menu = xmalloc(sizeof(int) * 2);
	}
	else
	{
		infohistory.length++;
		infohistory.node = xrealloc(infohistory.node, sizeof(char *) *(infohistory.length + 1));
		infohistory.file = xrealloc(infohistory.file, sizeof(char *) *(infohistory.length + 1));
		infohistory.pos = xrealloc(infohistory.pos, sizeof(int) *(infohistory.length + 1));
		infohistory.cursor = xrealloc(infohistory.cursor, sizeof(int) *(infohistory.length + 1));
		infohistory.menu = xrealloc(infohistory.menu, sizeof(int) *(infohistory.length + 1));
	}
	infohistory.node[infohistory.length] = xmalloc(strlen(node) + 1);
	strcpy(infohistory.node[infohistory.length], node);
	infohistory.file[infohistory.length] = xmalloc(strlen(file) + 1);
	strcpy(infohistory.file[infohistory.length], file);
	infohistory.pos[infohistory.length] = pos;
	infohistory.cursor[infohistory.length] = cursor;
	infohistory.menu[infohistory.length] = menu;
}

/*
 * Delete last history entry
 */
void
dellastinfohistory()
{
	if (infohistory.length)
	{
		if (infohistory.node[infohistory.length])
		{
			xfree(infohistory.node[infohistory.length]);
			infohistory.node[infohistory.length] = 0;
		}
		if (infohistory.file[infohistory.length])
		{
			xfree(infohistory.file[infohistory.length]);
			infohistory.file[infohistory.length] = 0;
		}
		if (infohistory.length)
			infohistory.length--;
		if (infohistory.length)
		{
			infohistory.node = xrealloc(infohistory.node, sizeof(char *) *(infohistory.length + 1));
			infohistory.file = xrealloc(infohistory.file, sizeof(char *) *(infohistory.length + 1));
			infohistory.pos = xrealloc(infohistory.pos, sizeof(int) *(infohistory.length + 1));
			infohistory.cursor = xrealloc(infohistory.cursor, sizeof(int) *(infohistory.length + 1));
			infohistory.menu = xrealloc(infohistory.menu, sizeof(int) *(infohistory.length + 1));
		}
		else
		{
			if (infohistory.node)
			{
				xfree(infohistory.node);
				infohistory.node = 0;
			}
			if (infohistory.file)
			{
				xfree(infohistory.file);
				infohistory.file = 0;
			}
			if (infohistory.pos)
			{
				xfree(infohistory.pos);
				infohistory.pos = 0;
			}
			if (infohistory.cursor)
			{
				xfree(infohistory.cursor);
				infohistory.cursor = 0;
			}
			if (infohistory.menu)
			{
				xfree(infohistory.menu);
				infohistory.menu = 0;
			}
		}
	}
}

void
clearfilenameprefix()
{
	if (filenameprefix)
	{
		xfree(filenameprefix);
		filenameprefix = 0;
	}
}