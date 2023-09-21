// https://codereview.stackexchange.com/questions/63427/simple-key-value-store-in-c
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "command.h"
#include "kvs.h"
#include "main.h"

int checKey(const char *chave){
  if (strncmp(chave, "cpf_", 4) == 0)  {
      return 1;
  }
  else if(strncmp(chave, "data_", 4) == 0){
    return 2;
  }
  return 0;
  
}
int main(int argc, char *argv[]) {

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  if (luaL_dofile(L, "extend.lua")) {
    fprintf(stderr, "Erro ao carregar o arquivo Lua: %s\n",
            lua_tostring(L, -1));
    return -1;
  }

  char input[100];
  KVSstore *banco;
  // Inicializando KVS
  banco = kvs_create();

  printf("Execute algum comando. Exemplos:\n");
  printf("- ADD abc 1234 ---> Adiciona o valor 1234 na chave abc\n");
  printf("- GET abc ---> Retorna o valor da chave abc se existir\n");
  printf("- EXIT --->  sair do programa\n");

  for (;;) {
    // Imprime e mantém na mesma linha
    printf("> ");
    fflush(stdout);
    gets(input);

    Command command = get_command(input);

    // se igual, strcmp == 0
    if (!strcmp(command.command, "EXIT")) {
      printf("Inté!\n");
      break;
    } else if (!strcmp(command.command, "ADD")) {

      if(checKey(command.key)==1){
        lua_getglobal(L, "validaCPF");  /* function to be called */
        lua_pushstring(L, command.value);   /* push argument */
          if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
          fprintf(stderr, "Erro ");
          break;
        }
        int r = lua_tonumber(L, -1);

        printf("%d\n",r);
        lua_pop(L, 1);
        if(r == 0){
          lua_pop(L,1);
          printf("\nCPF valido, inserindo no banco\n");
          lua_getglobal(L, "getCPF");  /* function to be called */
          lua_pushstring(L, command.value);   /* push 1st argument */
          if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
          fprintf(stderr, "Erro CPF 2");
          break;
        }
          char* cpfF= lua_tostring(L, -1);

          lua_pop(L, 1);
          kvs_put(banco, command.key, cpfF);
        }
        else{
        printf("CPF invalido\n");
        }
      }else if(checKey(command.key)==2){
        lua_getglobal(L, "validaData");  /* function to be called */
        lua_pushstring(L, command.value);   /* pushargument */
        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
          fprintf(stderr, "Erro Data");
          break;
        }
        int r = lua_toboolean(L, -1);

        lua_pop(L, 1);
        if(r==1){
          lua_getglobal(L, "getData");  /* function to be called */
          lua_pushstring(L, command.value);   /* push 1st argument */
          if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
          fprintf(stderr, "Erro Data 2");
          break;
        }
          char *dataF = lua_tostring(L, -1);
          lua_pop(L, 1);
          printf("\nData valida, inserindo no banco\n");
          kvs_put(banco, command.key, dataF);
        }
        else{
        printf("Data invalida\n");
        }
      }else{
        printf("\nFormato de chave invalido\n");
      }
      
    } else if (!strcmp(command.command, "GET")) {
      char *value = kvs_get(banco, command.key);
      printf(value);
      printf("\n");
    } else {
      printf("Comando não reconhecido!\n");
    }
  }

  kvs_destroy(banco);
  return 0;
}