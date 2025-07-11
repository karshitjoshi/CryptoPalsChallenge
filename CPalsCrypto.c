#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// SIGNATURES
int helperBase64(int input,int iter);
char * HexBase64(char *data,char *dest);
int activator(int ch,char **arguments);

// BASE64 CONVERTORS

int helperBase64(int input,int iter){
	if(input == 0 && (iter > 1)){
		return 61;
	} else if(input < 26){
		return 65 + input - 0;
	} else if(input < 52){
		return 97 + input - 26;
	} else if(input < 62){
		return 48 + input - 52;
	} else if(input == 62){
		return 43;
	} else if(input == 63){
		return 47;
	}
	return -1;
}

char * HexBase64(char *data,char *dest){
	int partConverter[4];
	for(int i = 0;i < (ceil(strlen(data)/3)+1);i++){
		if((data[3*i + 0] != 0) && (data[3*i + 1] != 0) && (data[3*i + 2] != 0)){
			partConverter[0] = (data[3*i] & 252) >> 2; // Bitmask 0b11111100 over data[3*i] similarly the rest.
			partConverter[1] = ((data[3*i] & 3) << 4) | ((data[(3*i)+1] & 240) >> 4);
			partConverter[2] = ((data[(3*i) + 1] & 15) << 2) | ((data[(3*i+2)] & 192) >> 6);
			partConverter[3] = ((data[(3*i) + 2]) & 63);
		}
		if((data[3*i + 0] != 0) && (data[3*i + 1] != 0) && (data[3*i + 2] == 0)){
			partConverter[0] = (data[3*i] & 252) >> 2; // Bitmask 0b11111100 over data[3*i] similarly the rest.
			partConverter[1] = ((data[3*i] & 3) << 4) | ((data[(3*i)+1] & 240) >> 4);
			partConverter[2] = ((data[(3*i) + 1] & 15) << 2) | ((data[(3*i+2)] & 192) >> 6);
			partConverter[3] = 0;
		}
		if((data[3*i + 0] != 0) && (data[3*i + 1] == 0)){
			partConverter[0] = (data[3*i] & 252) >> 2; // Bitmask 0b11111100 over data[3*i] similarly the rest.
			partConverter[1] = ((data[3*i] & 3) << 4) | ((data[(3*i)+1] & 240) >> 4);
			partConverter[2] = 0;
			partConverter[3] = 0;
		}
		for(int j = 0;j < 4;j++){
			dest[4*i + j] = helperBase64(partConverter[j],j);
		}
	}
	return dest;
}

// HEX CONVERTORS

int ToHex(char *data,char *dest){
	for (int i = 0; i < strlen(data); ++i) {
		sprintf(dest+(sizeof(char)*i*2),"%x",data[i]);
	}
	return 0;
}

int FromHex(char *data,char *dest){
	for (int i = 0; i < (strlen(data) / 2); i++) {
   		sscanf(data + 2*i, "%02x",(unsigned int *)(&dest[i]));
	}
	return 0;
}

// CHOICE MAKERS

int activator(int ch,char **arguments){
	if(ch == 0){
		return 1;
	} else if(ch == 1){
		char *output = malloc(sizeof(char)*2*strlen(arguments[2]));
		HexBase64(arguments[2],output);
		printf("%s\n",output);
		free(output);
		return 0;
	} else if(ch == 2){
		char *output = malloc(sizeof(char)*strlen(arguments[2])*2);
		ToHex(arguments[2],output);
		printf("%s\n",output);
		free(output);
	} else if(ch == 3){
		char *output = malloc(sizeof(char)*strlen(arguments[2]));
		FromHex(arguments[2],output);
		printf("%s\n",output);
		free(output);
	} else if(ch == 31){
		char *output = malloc(sizeof(char)*strlen(arguments[2])*2);
		char *output2 = malloc(sizeof(char)*strlen(arguments[2])*2);
		FromHex(arguments[2],output);
		HexBase64(output,output2);
		printf("%s\n",output2);
		free(output);free(output2);
	}
	return 0;
}

int main(int argc,char **argv){
	if (argc == 1 || activator(atoi(argv[1]),argv)){
		puts("[*] Non Meaningful Output, Please Retry With Valid Input");
	}
	return 0;
}