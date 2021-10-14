#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

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
                int justCurr;
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

static size_t
write_data(void *p, size_t s, size_t nm, void *st)
{
        size_t w = fwrite(p, s, nm, (FILE *)st);
        return w;
}

static char *
prep_url(char *end, struct EndPoints ep)
{
        char bufurl[100];
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
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/documents/");
                        strcat(bufurl, ep.Documents.docNum);
                        strcat(bufurl, ".");
                        strcat(bufurl, ep.Documents.format);
                        printf("bufurl : %s\n", bufurl);
                }
        } else if(!strcmp(end, "PubInsDocs")) {
                if(ep.PubInsDocs.justDocs == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/public-inspection-documents.");
                        strcat(bufurl, ep.PubInsDocs.format);
                } else if(ep.PubInsDocs.justCurr == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/current.");
                        strcat(bufurl, ep.PubInsDocs.format);
                } else {
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/public-inspection-documents/");
                        strcat(bufurl, ep.PubInsDocs.docNum);
                        strcat(bufurl, ".");
                        strcat(bufurl, ep.PubInsDocs.format);
                }
        } else if(!strcmp(end, "Agencies")){
                if(ep.Agencies.agencies == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/agencies");
                        /* NOTE: Format is not an option on agencies and SuggSearches, for some odd reason */ 
                } else {
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/agencies/");
                        strcat(bufurl, ep.Agencies.slug);
                }
        } else if(!strcmp(end, "SuggSearches")){
                if(ep.SuggSearchs.suggSearch == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/suggested_searches");
                } else {
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/suggested_searches/");
                        strcat(bufurl, ep.SuggSearchs.slug);
                }
        }
        char *url = malloc(strlen(bufurl) + 1);
        strcpy(url, bufurl);
        return url;
}

void
get_page(char *end, char *fmt, struct EndPoints ep)
{
        CURL *ch;
        FILE *fp = fopen("data.json", "wb");
        char *url; 
        url = prep_url(end, ep);

        ep.Documents.format = fmt;

/*        prep_url(bufurl, "%s/%i.%s", ) */
        
        printf("url : %s\n", url);
        curl_global_init(CURL_GLOBAL_ALL);
        ch = curl_easy_init();
        curl_easy_setopt(ch, CURLOPT_URL, url);
        curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_data);
        if(fp){
                curl_easy_setopt(ch, CURLOPT_WRITEDATA, fp);
                curl_easy_perform(ch);
        }
        fclose(fp);
        free(url);
        curl_easy_cleanup(ch);
        curl_global_cleanup();

}


static char *
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

static char *
find_element(char *elementName, char *buffer)
{
        cJSON *node = NULL;
        cJSON *element = NULL;


        char *string = NULL;
        node = cJSON_Parse(buffer);
        if(node == NULL) return string;

        element = cJSON_GetObjectItemCaseSensitive(node, elementName);
        if(cJSON_IsString(element) && (element->valuestring != NULL)){
                string = element->valuestring;
                printf("element: %s", string);
        } else {
                printf("element not found on iteration\n");
        }
        return string;
}

static char *
find_loop_element(char *elementName, char *buffer)
{
        const cJSON *element = NULL;
        const cJSON *arrayname = NULL;
        cJSON *node = NULL;

        char *string = NULL;
        node = cJSON_Parse(buffer);

        if(node == NULL) return string;
        arrayname = cJSON_GetObjectItemCaseSensitive(node, "results");
        cJSON_ArrayForEach(element, arrayname){
                cJSON *nstElement = cJSON_GetObjectItemCaseSensitive(element, elementName);
                printf("%s\n", nstElement->valuestring);
        }
        return string;
}

int
main(int argc, char *argv[])
{
        struct EndPoints ep;
/*
        for(int i = 0; i < argc; i++){
                if(!strcmp("--acgencies", argv[i])){
                        if(strcmp("1", argv[i+1])){
                                ep.Agencies.agencies = 1;
                        } else ep.Agencies.agencies = 0; continue; 
                } else if(!strcmp("--documents-all", argv[i])){
                        if(!strcmp("1", argv[i+1])){
                                ep.Documents.justDocs = 1;
                        } else ep.Documents.justDocs = 0; continue; 
                } else if(!strcmp("--documents-number", argv[i])){
                        strcpy(ep.Documents.docNum, argv[i+1]);
                } else if(!strcmp("--public-inspection-all", argv[i])){
                        if(!strcmp("1", argv[i+1])){
                                ep.PubInsDocs.justDocs = 1;
                        } else ep.PubInsDocs.justCurr = 0; continue;
                } else if(!strcmp("--public-inspection-number", argv[i])){
                        strcpy(ep.PubInsDocs.docNum, argv[i+1]);
                } else if(!strcmp("--public-inspection-current", argv[i])){
                        if(!strcmp("1", argv[i+1])){
                                ep.PubInsDocs.current = 1;
                        } continue;
                }
        }
*/



        char *buff = NULL;

        ep.Documents.format = "json";

        ep.Documents.justDocs = 1;
        ep.Documents.Facets.facets = 0;
//        ep.Documents.docNum = "2021-21908";
        printf("ep.Documents.justDocs is equal to %i\n", ep.Documents.justDocs);
        get_page("Documents", "json", ep);

        buff = read_to_buff();
        if(ep.Documents.justDocs == 1){
                find_loop_element("html_url", buff);
        } else if(ep.Agencies.agencies == 1){
                find_loop_element("html_url", buff);
        } else if(ep.PubInsDocs.justDocs == 1){
                find_loop_element("html_url", buff);
        } else if(ep.SuggSearchs.suggSearch == 1){
                find_loop_element("html_url", buff);
        } else {
                find_element("html_url", buff);
        }

        free(buff);
}
