/*
 * configloader.c
 *
 *  Created on: Jan 7, 2018
 *      Author: heng
 */

//#include <assert.h>
#include <ctype.h>
#include "configloader.h"

#define TRIM_LEFT 0x01
#define TRIM_RIGHT 0x02
#define TRIM_ALL 0x03


int set_zero(char *line);
int trim(char *out,const char *in,unsigned char flag);
struct hash_t *create_hash(const char *line);


void conf_add(struct conf_t *conf,struct hash_t *hast);


int set_zero(char *line){
    size_t len=strlen(line);
    size_t ret=len;
    for(size_t i=0;i<len;i++){

        if(line[i]=='#'){
            for(size_t k=i;k<len;k++){
                line[k]=0;
                ret--;
            }

        }
        else if(line[i]=='\r'||line[i]=='\n'){
            line[i]=0;
            ret--;
        }
    }
    return ret;

}
int trim(char *out,const char *in,unsigned char flag){
    size_t inlen=strlen(in);
    char *newline=malloc(inlen+1);
    char *line=newline;
    strcpy(line,in);
    if((flag&0x02)==0x02){
        while(isspace(line[inlen-1])){
            line[inlen-1]=0;
            inlen--;
        }
    }

    if((flag&0x01)==0x01){
        while(isspace(*line++)){
            inlen--;
        }
        line--;
    }

    strcpy(out,line);
    free(newline);
    return inlen;

}
struct hash_t *create_hash(const char *line){
    if(line==NULL) return NULL;
    size_t linesize=strlen(line);
    if(linesize==0) return NULL;

    struct hash_t *kv=(struct hash_t *)calloc(sizeof(struct hash_t),1);

    size_t split_flag_idx=linesize;
    for(size_t i=0;i<linesize;i++){
        if(line[i]=='='){
            split_flag_idx=i;

            break;
        }

    }
    kv->key=(char *)calloc(split_flag_idx+1,1);

    char *key=(char *)calloc(split_flag_idx+1,1);
    strncpy(key,line,split_flag_idx);
    trim(kv->key,key,TRIM_RIGHT);
    free(key);

    if(linesize==split_flag_idx){
        kv->value=NULL;
    }else{
        const char *value=line+split_flag_idx+1;
        kv->value=(char *)calloc(linesize-split_flag_idx,1);

        trim(kv->value,value,TRIM_LEFT);
    }


    return kv;



}
void conf_add(struct conf_t *conf,struct hash_t *hash){
    if(hash==NULL) return;
    if(conf->head==NULL){
        conf->head=hash;
        conf->end=hash;
        conf->size++;
        return;
    }
    if(conf->size==1){
        conf->head->next=hash;
        conf->end=hash;

    }
    else{
        conf->end->next=hash;
        conf->end=hash;
    }
    conf->size++;

}
int conf_parse(struct conf_t **out,const char *filename){

   struct conf_t *conf=(struct conf_t *)calloc(sizeof(struct conf_t),1);
   FILE *fp=NULL;
   //打开配置文件，返回FILE指针

    if((fp=fopen(filename,"rb")) == NULL){
        #ifndef NDEBUG
        printf("打开%s出错!\n",filename);
        #endif // NDEBUG
        free(conf);

        return 1;
    }

    ssize_t size=0;//存放 当前行 的长度（包括“\n”）

    size_t len = 0;//文件总长度

    char * line = NULL;//存放 当前行 的字符串


    while((size=getline(&line,&len,fp))!=-1){
        if(size>1){
            size_t proclen=set_zero(line);
            if(proclen>0){
                char *trimed=calloc(size+1,1);
                trim(trimed,line,TRIM_ALL);
                if(strlen(trimed)>0){
                    struct hash_t *kv=create_hash(trimed);
                    conf_add(conf,kv);
                }

                free(trimed);
            }
        }


        len=0;
        size=0;

        //printf("k:%s,v:%s \n",kv->key,kv->value);
        free(line);
        line=NULL;
    }

    fclose(fp);
    *out=conf;
    return 0;


}
void  conf_each(conf_each_func func,struct conf_t *conf,void  *params){

    struct hash_t *kv=conf->head;
    while(kv!=NULL){
        if(func(kv,params)!=EACH_CONTINUE) break;
        kv=kv->next;
    }
}

const char *conf_get_value(struct conf_t* conf,const char *key){


    struct hash_t *kv=conf->head;
    while(kv!=NULL){
        if(kv->key==NULL) continue;

        if(strcmp(key,kv->key)==0||kv->key==key)
            return kv->value;

        kv=kv->next;
    }
    return NULL;
}

int conf_destory(struct conf_t *conf){
    //printf("free start !\n");
    struct hash_t *kv=conf->head;
    while(kv!=NULL){
        struct hash_t *cur=kv->next;
        //printf("key:%s,value:%s \n",kv->key,kv->value);
        if(kv->key) free(kv->key);
        if(kv->value) free(kv->value);
        free(kv);
        //kv=NULL;
        kv=cur;
    }
    free(conf);
    //printf("free end !\n");
    return 0;
}
int conf_load(struct sys_config_t *params,const char *filename){
	struct conf_t *config;
	if(conf_parse(&config,filename)){
		return -1;
	}
	const char *hvalue=NULL;

	hvalue = conf_get_value(config,SENS_ADDR_KEY);
	if(hvalue!=NULL)
		strcpy(params->sens_addr,hvalue);
	else
		strcpy(params->sens_addr,SENS_DEFAULT_ADDR);


	hvalue = conf_get_value(config,SENS_PORT_KEY);
	if(hvalue!=NULL)
		params->sens_port=atoi(hvalue);
	else
		params->sens_port=SENS_DEFAULT_PORT;

	hvalue = conf_get_value(config,SENS_CONNECTION_SIZE_KEY);
	if(hvalue!=NULL)
		params->sens_connection_size=atoi(hvalue);
	else
		params->sens_connection_size=SENS_DEFAULT_CONNECTION_SIZE;

	hvalue = conf_get_value(config,SENS_RECV_TIMEOUT_KEY);
	if(hvalue!=NULL)
		params->sens_recv_timeout=atoi(hvalue);
	else
		params->sens_recv_timeout=SENS_DEFAULT_RECV_TIMEOUT;

	hvalue = conf_get_value(config,SENS_SEND_TIMEOUT_KEY);
	if(hvalue!=NULL)
		params->sens_send_timeout=atoi(hvalue);
	else
		params->sens_send_timeout=SENS_DEFAULT_SEND_TIMEOUT;
	//
	hvalue = conf_get_value(config,CTRL_RECV_TIMEOUT_KEY);
	if(hvalue!=NULL)
		params->ctrl_recv_timeout=atoi(hvalue);
	else
		params->ctrl_recv_timeout=CTRL_DEFAULT_RECV_TIMEOUT;

	hvalue = conf_get_value(config,CTRL_SEND_TIMEOUT_KEY);
	if(hvalue!=NULL)
		params->ctrl_send_timeout=atoi(hvalue);
	else
		params->ctrl_send_timeout=CTRL_DEFAULT_SEND_TIMEOUT;
	//

	hvalue=conf_get_value(config,CTRL_ADDR_KEY);
	if(hvalue!=NULL)
		strcpy(params->ctrl_addr,hvalue);
	else
		strcpy(params->ctrl_addr,CTRL_DEFAULT_ADDR);

	hvalue=conf_get_value(config,CTRL_PORT_KEY);
	if(hvalue!=NULL)
		params->ctrl_port=atoi(hvalue);
	else
		params->ctrl_port = CTRL_DEFAULT_PORT;

	hvalue=conf_get_value(config,CTRL_CONNECTION_SIZE_KEY);
	if(hvalue!=NULL)
		params->ctrl_connection_size=atoi(hvalue);
	else
		params->ctrl_connection_size=CTRL_DEFAULT_CONNECTION_SIZE;

	hvalue =conf_get_value(config,SYSTEM_LOG_KEY);
	if(hvalue!=NULL)
		strcpy(params->sys_log_file,hvalue);
	else
		strcpy(params->sys_log_file,SYSTEM_DEFUALT_LOG);

	hvalue =conf_get_value(config,SYSTEM_SERVER_NAME_KEY);
	if(hvalue!=NULL)
		strcpy(params->sys_server_name,hvalue);
	else
		strcpy(params->sys_server_name,SYSTEM_DEFUALT_SERVER_NAME);

	hvalue =conf_get_value(config,SENS_AUTH_TYPE_KEY);
	if(hvalue!=NULL){
		params->sens_auth_type=atoi(hvalue);

	}
	else
		params->sens_auth_type=SENS_DEFAULT_AUTH_TYPE;

	hvalue =conf_get_value(config,SYSTEM_DB_FILE_KEY);
	if(hvalue!=NULL){
		strcpy(params->sys_db_file,hvalue);

	}
	else
		strcpy(params->sys_db_file,SYSTEM_DEFUALT_DB_FILE);

	hvalue=conf_get_value(config,CTRL_AUTH_MODE_KEY);
	if(hvalue!=NULL){
		params->ctrl_auth_type=atoi(hvalue);

	}
	else
		params->ctrl_auth_type=CTRL_DEFUALT_AUTH_MODE;


	conf_destory(config);
	return 0;
}

