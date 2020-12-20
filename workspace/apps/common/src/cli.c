/*
 * cli.c
 *
 *  Created on: 30 Nov.,2020
 *      Author: Nikolas Karakotas
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <getopt.h>
#include <ctype.h>
#include <txm_module.h>
#include "cli.h"

const char *CLI_CMD_HELP = "help";
const char *CLI_CMD_MEMINFO = "meminfo";


static int cli_cmd_help(int args, char *argv[]){
	int opt;
	while((opt=getopt(args, argv, "h")) != -1){
    	switch(opt) {
    		default:
				printf("%s\r\n", optarg);
				puts("Usage: \r\n");
			break;
    	}
    }

	printf("%s\r\n", optarg);
	puts("Usage: \r\n");  	


	return 0;

}

extern TX_BYTE_POOL *malloc_get_pool(void);
static int cli_cmd_meminfo(int args, char *argv[]){

	ULONG available;
	ULONG fragments;
	TX_THREAD *first_suspended;
	ULONG suspended_count;
	TX_BYTE_POOL *next_pool;
	UINT status;


	int opt;
	while((opt=getopt(args, argv, ":m:")) != EOF){
    	switch(opt) {
			case 'm':
				printf("%s\r\n", optarg);
				/* Retrieve information about the previously created
				block pool "my_pool." */
				tx_byte_pool_info_get(malloc_get_pool(), 
					"memheap",
					&available, &fragments,&first_suspended, &suspended_count,
					&next_pool);
				printf("Available:\t\t%lu\nFragments:\t\t%lu\nFirst Suspended:\t%p\nSuspended Count:\t%lu\nNext Pool:\t\t%p\n", 
					available,fragments,first_suspended,suspended_count,next_pool);
			break;
			default:
				return -1;
				break;
    	}
    }

	return 0;

}


void cli_free(cli_t *ctx){
	free(ctx->in);
//TODO	free(cli->cmd);
	free(ctx);
}

cli_t *cli_new(void){

	cli_cmd_t *cmd;

	cli_t *ctx = malloc(sizeof(cli_t));
	
	ctx->cmd = malloc(sizeof(cli_cmd_t));
	ctx->cmd->name = (char*)CLI_CMD_HELP;
	ctx->cmd->callback = cli_cmd_help;
	ctx->cmd->next = NULL;

	cmd = malloc(sizeof(cli_cmd_t));
	cmd->name = (char*)CLI_CMD_MEMINFO;
	cmd->callback = cli_cmd_meminfo;
	cmd->next = NULL;

	ctx->cmd->next = cmd;

	ctx->in = calloc(1,STDIO_IN_MAX);
	return ctx;
}

int cli_register(cli_t *ctx, const char *name, cli_callback_t func){
	
	cli_cmd_t *iter;

	if(name && func && ctx){

		iter = ctx->cmd;
		while(iter->next != NULL){
			if(strcmp(iter->name,name) == 0 )
				return 1;
			iter = iter->next;
		}

		cli_cmd_t *cmd = malloc(sizeof(cli_cmd_t));
		cmd->name = (char*)name;
		cmd->callback = func;
		cmd->next = NULL;
		
		iter->next = cmd;

		return 0;
	}

	return 1;
}

char *cli_input(cli_t *ctx){

	int got_eof = 0;
	int args = 0;
	char *str;
	char *argv[STDIO_CMD_ARGS_MAX];

	memset(argv,0,STDIO_CMD_ARGS_MAX);
	memset(ctx->in,0,STDIO_IN_MAX);
	str = ctx->in;

	puts("\n$> ");
	
	while (!got_eof) {

		for (;;) {
			int c = getchar();
			putchar(c);

			if (c == EOF || ctx->in >= (ctx->in+STDIO_IN_MAX)) {
				got_eof = 1;
				break;
			} else if (c == '\n' || c == '\r') {
				got_eof = 1;
				break;
			} else {
				*(str++) = (char)c;
			}
		}

	}


	if(ctx->in){

		char *token = strtok(ctx->in, " ");

		while( token != NULL ) {
			argv[args] = strdup(token);
			args++;
		  	token = strtok(NULL, " ");
		}

		cli_cmd_t *cmd = ctx->cmd;
		optind = 1;
		opterr = 0;

		while(cmd){
			if(argv[0] && strcmp(cmd->name,argv[0]) == 0 )
				cmd->callback(args, argv);
			cmd = cmd->next;

		}

		while(args){
			free(argv[args-1]);
			args--;
		}

	}
	
	return ctx->in;

}