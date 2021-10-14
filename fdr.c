#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "fdr.h"



/* proto defs */
static size_t write_data(void *p, size_t s, size_t nm, void *st);

static size_t
write_data(void *p, size_t s, size_t nm, void *st)
{
        size_t w = fwrite(p, s, nm, (FILE *)st);
        return w;
}

static char *
prep_url(char *end, EndPoints ep)
{
        char bufurl[100];
        if(!strcmp(end, "Documents")){
                if(ep.Documents.justDocs == 1){
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/documents.%s", ep.format);
                } else if(ep.Documents.Facets.facets == 1){
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/facets/%s", ep.Documents.Facets.facet);
                } else {
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/documents/%s.%s",
                                        ep.Documents.docNum, ep.format);
                }
        } else if(!strcmp(end, "PubInsDocs")) {
                if(ep.PubInsDocs.justDocs == 1){
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/public-inspection-documents.%s", ep.format);
                } else if(ep.PubInsDocs.justCurr == 1){
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/public-inspection-documents/current.%s", ep.format);
                } else {
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/public-inspection-documents/%s.%s", ep.PubInsDocs.docNum, ep.format);
                }
        } else if(!strcmp(end, "Agencies")){
                if(ep.Agencies.agencies == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/agencies");
                        /* NOTE: Format is not an option on agencies and SuggSearches, for some odd reason */ 
                } else {
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/agencies/%s", ep.Agencies.slug);
                }
        } else if(!strcmp(end, "SuggSearches")){
                if(ep.SuggSearchs.suggSearch == 1){
                        strcpy(bufurl, "https://www.federalregister.gov/api/v1/suggested_searches");
                } else {
                        sprintf(bufurl, "https://www.federalregister.gov/api/v1/suggested_searches/%s", ep.SuggSearchs.slug);
                }
        }
        char *url = malloc(strlen(bufurl) + 1);
        strcpy(url, bufurl);
        return url;
}

void
get_page(char *end, char *fmt, EndPoints ep)
{
        CURL *ch;
        FILE *fp = fopen("data.json", "wb");
        char *url; 
        url = prep_url(end, ep);


/*        prep_url(bufurl, "%s/%i.%s", ) */
        
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

static int
safty_checking(EndPoints ep){
        int status;
        if(ep.format == NULL){
                fprintf(stderr, "ERROR: The code will segfult if ep.format is unset!!!!\n\n\nCurrent set : %s\n", ep.format);
                status = 1;
        }
        return status;
}

char *
get_urls(EndPoints ep, char *cat, char *elementName)
{
        char *buff = NULL;


        if(safty_checking(ep) == 1){
                return NULL;
        }

        get_page(cat, ep.format, ep);

        buff = read_to_buff();

        if(ep.Documents.justDocs == 1 || ep.Agencies.agencies == 1 || ep.PubInsDocs.justDocs == 1 || ep.PubInsDocs.justCurr == 1 || ep.SuggSearchs.suggSearch == 1){
                find_loop_element(elementName, buff);
        } else {
                find_element(elementName, buff);
        } 


        free(buff); 
        return cat;
}

