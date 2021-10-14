/* First Define your usaul headers */
#include <stdio.h>
/* Now include the federal register API header */
#include "../fdr.h"

int
main(int argc, char *argv[])
{
        /* Define the EndPoints struct */ 
        EndPoints ep;

        /* You need to define at least fmt or interal error will rise
         * Pick a catigory and start fleshing out the struct accordingly */
        ep.format = "json";
        /* You should define all the elements of the struct, you can get 
         * away with it in some cases but it is better to define them 
         * to avoid segmentation faults */
        ep.Documents.justDocs = 1;
        ep.Documents.Facets.facets = 0; 
        ep.Documents.Facets.facet = NULL;
        ep.Documents.docNum = NULL;

        /* That should be enough so now you can call the main function to
         * download the urls. get_urls needs a struct, ep, a catigory, 
         * "Documents", "PubInsDocs", "Agencies", "SuggSearches"
         * Documents is the basic call to start downloading documents
         * PubInsDocs are public inspection documents
         * Agencies, this is just agency data
         * SuggSearches, Suggested searches */ 
        get_urls(ep, "Documents", "html_url"); /* support for more element searching is soon */


}
