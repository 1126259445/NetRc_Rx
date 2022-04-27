/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "router.h"

#include <esp_http_server.h>
#include "User_NvsData.h"

static const char *TAG="USER_HTTP_Sever";




static wifi_config_t Wifi_info;


char home_html[1500] = {" \
<!DOCTYPE html> \
<html lang=\"en\"> \
<head>  \
	<meta charset=\"UTF-8\">  \
	<title>ESP8266</title>  \
		\
</head> \
<style> \
#myHeader { \
    padding-top: 250px; \
    padding: 180px; \
    color: black; \
    text-align: center; \
} \
</style> \
	<body bgcolor=lightblue>  \
    	<div id=myHeader>  \
        	<h1>NetRc设置</h1> \
		<form action=\"wifi_set\"> \
			WiFi名称： \
			<input type=\"text\" name=\"ssid\" placeholder=\"请输入您WiFi的名称\"> \
		    <br> \
			WiFi密码： \
			<input type=\"text\" name=\"password\" placeholder=\"请输入您WiFi的密码\"> \
			<input type=\"submit\" value=\"连接\"> \
			<br><br> \
			订阅ID： \
			<input type=\"text\" name=\"subid\" placeholder=\"请输入发射端的ID\">  \
			<input type=\"submit\" value=\"设置\"> \
		</form> \
            <br><br> \
		<form action=\"safe_ch_set\"> \
  		失控保护: 通道1<input type=\"number\" name=\"safe_ch1\" value= \"1500\" min=\"1000\" max=\"2000\"> \
			    通道2<input type=\"number\" name=\"safe_ch2\" value= \"1500\" min=\"1000\" max=\"2000\"> \
			    通道3<input type=\"number\" name=\"safe_ch3\" value= \"1500\" min=\"1000\" max=\"2000\">	\
                通道4<input type=\"number\" name=\"safe_ch4\" value= \"1500\" min=\"1000\" max=\"2000\"> \
			    通道5<input type=\"number\" name=\"safe_ch5\" value= \"1500\" min=\"1000\" max=\"2000\"> \
  		<input type=\"submit\" value=\"设置\"> \
		</form> \
		<br><br> \
		系统设置： \
		<br> \
		<form action=\"sys_reset\"> \
  		<input type=\"submit\" value=\"重启\"> \
		</form> \
	</body> \
</html> \
"} ;




esp_err_t sys_reset_get_handler(httpd_req_t *req)
{
     ESP_LOGI(TAG, "sys_reset_get_handler");
     esp_restart();
     return ESP_OK;
}

esp_err_t safe_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "safe Found URL query => %s", buf);

            char safe_ch[10] = {0};
            /* Get value of expected key from query string */
             if (httpd_query_key_value(buf, "safe_ch1", safe_ch, 9) == ESP_OK)
             {  
                  if(strlen(safe_ch) != 0)
                  {
                        User_HttpSeverInfo.safe_ch_val[0] = atoi(safe_ch);
                        ESP_LOGI(TAG, "safe_ch1 = %s", safe_ch);
                  }
                  else
                  {
                        ESP_LOGI(TAG, "safe_ch is NULL");
                  }         
             }

                memset(safe_ch,0,10);
              if (httpd_query_key_value(buf, "safe_ch2", safe_ch, 9) == ESP_OK)
             {  
                  if(strlen(safe_ch) != 0)
                  {
                        User_HttpSeverInfo.safe_ch_val[1] = atoi(safe_ch);
                        ESP_LOGI(TAG, "safe_ch2 = %s", safe_ch);
                  }
                  else
                  {
                        ESP_LOGI(TAG, "safe_ch is NULL");
                  }         
             }

            memset(safe_ch,0,10);
            if (httpd_query_key_value(buf, "safe_ch3", safe_ch, 9) == ESP_OK)
             {  
                  if(strlen(safe_ch) != 0)
                  {
                        User_HttpSeverInfo.safe_ch_val[2] = atoi(safe_ch);
                        ESP_LOGI(TAG, "safe_ch3 = %s", safe_ch);
                  }
                  else
                  {
                        ESP_LOGI(TAG, "safe_ch is NULL");
                  }         
             }

             memset(safe_ch,0,10);
             if (httpd_query_key_value(buf, "safe_ch4", safe_ch, 9) == ESP_OK)
             {  
                  if(strlen(safe_ch) != 0)
                  {
                        User_HttpSeverInfo.safe_ch_val[3] = atoi(safe_ch);
                        ESP_LOGI(TAG, "safe_ch4 = %s", safe_ch);
                  }
                  else
                  {
                        ESP_LOGI(TAG, "safe_ch is NULL");
                  }         
             }

             memset(safe_ch,0,10);
             if (httpd_query_key_value(buf, "safe_ch5", safe_ch, 9) == ESP_OK)
             {  
                  if(strlen(safe_ch) != 0)
                  {
                        User_HttpSeverInfo.safe_ch_val[4] = atoi(safe_ch);
                        ESP_LOGI(TAG, "safe_ch5 = %s", safe_ch);
                  }
                  else
                  {
                        ESP_LOGI(TAG, "safe_ch is NULL");
                  }         
             }
            ESP_LOGI(TAG, "safe_ch = %d %d  %d  %d  %d", User_HttpSeverInfo.safe_ch_val[0],User_HttpSeverInfo.safe_ch_val[1],User_HttpSeverInfo.safe_ch_val[2],User_HttpSeverInfo.safe_ch_val[3],User_HttpSeverInfo.safe_ch_val[4]);
            NetRc_save_info(&User_HttpSeverInfo); //save all user info
        }
         free(buf);
    }
    return ESP_OK;
}

esp_err_t wifi_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "wifi Found URL query => %s", buf);
            char ssid[32] = {0};
            char password[32] = {0};
            char subid[32] = {0};
            /* Get value of expected key from query string */
            if ((httpd_query_key_value(buf, "ssid", ssid, sizeof(ssid)) == ESP_OK) && (httpd_query_key_value(buf, "password", password, sizeof(password)) == ESP_OK))
             {
                if(strlen(ssid) != 0 && strlen(password) != 0)
                {
                    router_wifi_save_info((uint8_t*)ssid,(uint8_t*)password);
                    ESP_LOGI(TAG, "HOME Found URL query parameter => ssid=%s", ssid);
                    ESP_LOGI(TAG, "HOME Found URL query parameter => password=%s", password);
                    memcpy(User_HttpSeverInfo.ssid,ssid,strlen(ssid));
                    memcpy(User_HttpSeverInfo.password,password,strlen(password));
                    
                }
                else
                {
                     ESP_LOGI(TAG, "Wifi Info is NULL");
                }
            }

             if (httpd_query_key_value(buf, "subid", subid, sizeof(subid)) == ESP_OK)
             {  
                  if(strlen(subid) != 0)
                  {
                    ESP_LOGI(TAG, "HOME Found URL query parameter => subid=%s", subid);
                    memcpy(User_HttpSeverInfo.subid,subid,strlen(subid));
                  }
                  else
                  {
                      ESP_LOGI(TAG, "Mac is NULL");
                  }         
             }

            NetRc_save_info(&User_HttpSeverInfo); //save all user info
        }
        free(buf);
    }
    return ESP_OK;
}

/* An HTTP GET handler */
esp_err_t home_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

httpd_uri_t sys_reset ={
    .uri       = "/sys_reset",
    .method    = HTTP_GET,
    .handler   = sys_reset_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
//    .user_ctx  = home_html
};

httpd_uri_t safe = {
    .uri       = "/safe_ch_set",
    .method    = HTTP_GET,
    .handler   = safe_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
//    .user_ctx  = home_html
};

httpd_uri_t wifi = {
    .uri       = "/wifi_set",
    .method    = HTTP_GET,
    .handler   = wifi_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
//    .user_ctx  = home_html
};

httpd_uri_t home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = home_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = home_html
};


httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &home);
        httpd_register_uri_handler(server, &wifi);
        httpd_register_uri_handler(server, &safe);
        httpd_register_uri_handler(server, &sys_reset);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

static httpd_handle_t server = NULL;

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}




void HttpSever_Init()
{
    nvs_handle out_handle;
    
    NetRc_Read_info(&User_HttpSeverInfo);

    server = start_webserver();
}
