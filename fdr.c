#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <curl/curl.h>

#include "jsonparse.h"

/* structs, enums, unions */
struct EndPoints {
        struct Documents {
                int justDocs; /* one or zero to tell if anything should be done; */
                char *docNum;
                // char *docNums, TODO: array? pointer array? int? 3d array?
                struct Facets {
                        int facets;
                        char *facet;
                } Facets;
                char *format; /* json or csv, not needed bec default json*/ 
        } Documents;
        struct PubInsDocs { /* Public Inspection Documents */
                int justDocs;
                char *docNum;
                // char *docNums, TODO: array? pointer array? int? 3d array?
                int current; /* just grab current? */ 
                char *format;
        } PubInsDocs;
        struct Agencies {
                int agencies;
                char *slug; /* get agency's details */ 
        } Agencies;
        struct SuggSearchs {
                int suggSearch;
                char *slug;
        } SuggSearchs;
} EndPoints;

/* proto defs */
static size_t write_data(void *p, size_t s, size_t nm, void *st);
static void get_page(char *end, char *fmt, struct EndPoints ep);


static size_t
write_data(void *p, size_t s, size_t nm, void *st)
{
        size_t w = fwrite(p, s, nm, (FILE *)st);
        return w;
}

static void
get_page(char *end, char *fmt, struct EndPoints ep)
{
        CURL *ch;
        FILE *fp = fopen("data.json", "wb");

        char bufurl[100]; /* make a dynamic buffer */  

        ep.Documents.format = fmt;
        printf("struct defined, format : %s\n", ep.Documents.format);
        printf("struct nums: ep.Documents.justDocs : %i\n", ep.Documents.justDocs);

/*        prep_url(bufurl, "%s/%i.%s", ) */
        if(!strcmp(end, "Documents")){
                if(ep.Documents.justDocs == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/documents.");
                        strcat(bufurl, ep.Documents.format);
                        printf("\n\n\n!!!!bufurl : %s!!!\n\n\n", bufurl);
                } else if(ep.Documents.Facets.facets == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/facets/");
                        strcat(bufurl, ep.Documents.Facets.facet);
                        printf("bufurl : %s\n", bufurl);
                } else {
                        strcat(bufurl, "https://www.federalregister.gov/api/v1/documents/");
                        strcat(bufurl, ep.Documents.docNum);
                        strcat(bufurl, ".");
                        strcat(bufurl, ep.Documents.format);
                        printf("bufurl : %s\n", bufurl);
                }
        } else {
                printf("Well Nothing fucking worked\n");
        }

        char *url = bufurl;

        curl_global_init(CURL_GLOBAL_ALL);
        ch = curl_easy_init();
        curl_easy_setopt(ch, CURLOPT_URL, url);
        curl_easy_setopt(ch, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_data);
        if(fp){
                curl_easy_setopt(ch, CURLOPT_WRITEDATA, fp);
                curl_easy_perform(ch);
        }
        fclose(fp);
        curl_easy_cleanup(ch);
        curl_global_cleanup();

}

char *
read_to_buff()
{
        char *source = NULL;
        FILE *fp = fopen("data.json", "r");

        if(fp != NULL){
                if(fseek(fp, 0L, SEEK_END) == 0){
                        long bufsize = ftell(fp);
                        if(bufsize == -1) fprintf(stderr, "Error: bufsize == -1\n");

                        source = malloc(sizeof(char) * (bufsize + 1));

                        if(fseek(fp, 0L, SEEK_SET) != 0) fprintf(stderr, "Did not go back to front of file\n"); 

                        size_t newLen = fread(source, sizeof(char), bufsize, fp);
                        if(ferror(fp) != 0){
                                fprintf(stderr, "Error reading the file!");
                        } else {
                                source[newLen++] = '\0';
                        }
                }
        }
        return source;
//        free(source); TODO: FREE THIS AT THE END
}

int
main()
{
        char *buff = NULL;

        printf("Hello!\n");
        struct EndPoints ep;
        printf("Struct defined!\n");
        ep.Documents.justDocs = 0;
        ep.Documents.Facets.facets = 0;
        ep.Documents.docNum = "2021-21908";
        printf("ep.Documents.justDocs is equal to %i\n", ep.Documents.justDocs);
        get_page("Documents", "json", ep);

        printf("reading to buff\n");
        buff = read_to_buff();
        printf("read to buff : %s\n", buff);
        printf("Now reading json stuff\n");
        find_element("html_url", buff);

        //free(buff);
}
