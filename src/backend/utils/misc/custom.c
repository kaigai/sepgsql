/*
 * custom.c
 *    Routines to register CustomXXXXMethods tables
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 */
#include "postgres.h"

#include "utils/custom.h"
#include "utils/hsearch.h"

static HTAB *custom_scan_methods = NULL;

typedef struct
{
	char		custom_name[CUSTOMNAME_MAX_LEN];
	const CustomScanMethods	   *scan_methods;
} CustomMethodsEntry;

/*
 * register a new custom-scan provider
 */
void
RegisterCustomScanMethods(const CustomScanMethods *methods)
{
	CustomMethodsEntry *entry;
	bool		found;

	if (custom_scan_methods == NULL)
	{
		HASHCTL		ctl;

		memset(&ctl, 0, sizeof(HASHCTL));
		ctl.keysize = NAMEDATALEN;
		ctl.entrysize = sizeof(CustomMethodsEntry);

		custom_scan_methods = hash_create("CustomScanMethods",
										  100, &ctl, HASH_ELEM);
	}
	Assert(strlen(methods->CustomName) <= CUSTOMNAME_MAX_LEN);
	entry = (CustomMethodsEntry *) hash_search(custom_scan_methods,
											   methods->CustomName,
											   HASH_ENTER, &found);
	if (found)
		ereport(ERROR,
				(errcode(ERRCODE_DUPLICATE_OBJECT),
				 errmsg("CustomScanMethods \"%s\" already exists",
						methods->CustomName)));
	entry->scan_methods = methods;
}

/*
 * Get the methods for a given name of custom-scan provider
 */
const CustomScanMethods *
GetCustomScanMethods(const char *custom_name, bool missing_ok)
{
	CustomMethodsEntry *entry = NULL;

	if (custom_scan_methods != NULL)
		entry = (CustomMethodsEntry *) hash_search(custom_scan_methods,
												   custom_name,
												   HASH_FIND, NULL);
	if (!entry)
	{
		if (missing_ok)
			return NULL;
		ereport(ERROR,
				(errcode(ERRCODE_UNDEFINED_OBJECT),
				 errmsg("CustomScanMethods \"%s\" was not registered",
						custom_name)));
	}
	return entry->scan_methods;
}
