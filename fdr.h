#ifndef FDR_H
#define FDR_H

/* structs, enums, unions */
typedef struct EndPoints {
        struct Documents {
                int justDocs; /* One or zero, one is all documents should be returned, and zero if you dont want this field */
                char *docNum; /* number of the document to return, int justDocs must be zero*/
                // char *docNums, TODO: array? pointer array? int? 3d array?
                struct Facets {
                        int facets; /* should we get facets? helper for char *facet for eaiser checking */
                        char *facet; /* specific facet to return, int facets must be zero */
                } Facets;
        } Documents;
        struct PubInsDocs { /* Public Inspection Documents */
                int justDocs; /* just all docs? */
                int justCurr; /* just the current ones */
                char *docNum; /* specific document to return */
                // char *docNums, TODO: array? pointer array? int? 3d array?
                int current; /* just grab current? */ 
        } PubInsDocs;
        struct Agencies {
                int agencies; /* all agencies */
                char *slug; /* get agency's details */ 
        } Agencies;
        struct SuggSearchs {
                int suggSearch; /* all suggested_searches */
                char *slug; /* specific suggested_searches */
        } SuggSearchs;
        char *format; /* either json or csv, csv not yet implemented */
} EndPoints;


char *get_urls(struct EndPoints ep, char *cat, char *elementName);

#endif
