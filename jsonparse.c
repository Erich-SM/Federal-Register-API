#include <stdio.h>
#include <cjson/cJSON.h>
#include "jsonparse.h"

struct EPFields {
        struct DocumentFields {
                char *abstract;
                char *action;
                char *agencies;
                char *agency_names;
                char *body_html_url;
                char *cfr_references;
                char *citation;
                char *comment_url;
                char *comments_close_on;
                char *correction_of;
                char *corrections;
                char *dates;
                char *disposition_notes;
                char *docket_id;
                char *docket_ids;
                char *document_number;
                char *effective_on;
                char *end_page;
                char *excerpts;
                char *exevutive_order_notes;
                char *exevutive_order_number;
                char *full_text_xml_url;
                char *html_url;
                char *images;
                char *json_url;
                char *mods_url;
                char *page_length;
                char *page_views;
                char *pdf_url;
                char *president;
                char *president_document_number;
                char *proclamation_number;
                char *public_inspection_pdf_url;
                char *publication_date;
                char *raw_text_url;
                char *regulation_id_number_info;
                char *regulation_id_numbers;
                char *regulation_dots_gov_info;
                char *regulation_dots_gov_url;
                char *significant;
                char *signing_date;
                char *start_page;
                char *subtype;
                char *title;
                char *toc_doc;
                char *topics;
                char *type;
                char *volume;
        } DocumentFields;
        struct AllDocFields {
                struct Fields {
                        char *abstract;
                        char *action;
                        char *agencies;
                        char *agency_names;
                        char *body_html_url;
                        char *cfr_references;
                        char *citation;
                        char *comment_url;
                        char *comments_close_on;
                        char *correction_of;
                        char *corrections;
                        char *dates;
                        char *disposition_notes;
                        char *docket_id;
                        char *docket_ids;
                        char *document_number;
                        char *effective_on;
                        char *end_page;
                        char *excerpts;
                        char *exevutive_order_notes;
                        char *exevutive_order_number;
                        char *full_text_xml_url;
                        char *html_url;
                        char *images;
                        char *json_url;
                        char *mods_url;
                        char *page_length;
                        char *page_views;
                        char *pdf_url;
                        char *president;
                        char *president_document_number;
                        char *proclamation_number;
                        char *public_inspection_pdf_url;
                        char *publication_date;
                        char *raw_text_url;
                        char *regulation_id_number_info;
                        char *regulation_id_numbers;
                        char *regulation_dots_gov_info;
                        char *regulation_dots_gov_url;
                        char *significant;
                        char *signing_date;
                        char *start_page;
                        char *subtype;
                        char *title;
                        char *toc_doc;
                        char *topics;
                        char *type;
                        char *volume;
                } Fields;
        } AllDocFields;
} EPFields;

char *
find_element(char *elementName, char *buffer)
{
        printf("setting elements\n");
        cJSON *node = NULL;
        cJSON *element = NULL;

        printf("parsing node\n");
        char *string = NULL;
        node = cJSON_Parse(buffer);
        if(node == NULL) return string;

        printf("finding element\n");
        element = cJSON_GetObjectItemCaseSensitive(node, elementName);
        if(cJSON_IsString(element) && (element->valuestring != NULL)){
                string = element->valuestring;
                printf("element: %s", string);
        } else {
                printf("element not found on iteration\n");
        }
        return string;
}
