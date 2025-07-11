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

// FILE FUNCTIONS

long SizeFile(char *filename){
	FILE *fp;
	fp = fopen(filename,"rb");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fclose(fp);
	return size;
}

int ReadFile(char *filename,char *dest){
	FILE *fp;
	fp = fopen(filename,"rb");
	fread(dest,sizeof(char),SizeFile(filename),fp);
	fclose(fp);
	return 0;
}

// XOR BRUTEFORCE

int BruteForceXOR(char *data){
	int broken = 0;
	char *output = malloc(sizeof(char) * strlen(data));
	for(unsigned int i = 0;i < 256;i++){
		printf("WE ARE HERE %d \n",i);
		for(int j = 0;j < strlen(data);j++){
			if((((data[j] ^ i) > 127) || ((data[j] ^ i) < 32)) && (data[j] != '\n')){
				broken = 1;
				break;
			} else {
				if(data[j] == '\n'){
					output[j] = data[j];
				} else {
					output[j] = data[j] ^ i;
				}
			}
		}
		if(broken != 1){
			printf("%s %x\n",output,i);
		} else {
			broken = 0;
		}
	}
	free(output);
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
	} else if(ch == 4){
		char *key = malloc(sizeof(char)*strlen(arguments[2])*2);
		char *buffer = malloc(sizeof(char)*strlen(arguments[3])*2);
		FromHex(arguments[3],buffer);
		FromHex(arguments[2],key);
		for(int i = 0;i < strlen(arguments[3])/2;i++){
			printf("%x",(buffer[i] ^ key[i % strlen(key)]));
		}
		printf("\n");
		free(key);free(buffer);
	} else if(ch == 5){
		char *output = malloc(sizeof(char)*SizeFile(arguments[2]));
		ReadFile(arguments[2],output);
		printf("%s",output);
		printf("\n");
		free(output);
	} else if(ch == 37){
		char *output = malloc(sizeof(char)*strlen(arguments[2]));
		FromHex(arguments[2],output);
		BruteForceXOR(output);
		free(output);
	} else if(ch == 57){
		char *output = malloc(sizeof(char)*SizeFile(arguments[2]));
		char *output1 = malloc(sizeof(char)*SizeFile(arguments[2]));
		ReadFile(arguments[2],output);
		FromHex(output,output1);
		BruteForceXOR(output1);
		free(output);
		free(output1);
	}
	return 0;
}

int main(int argc,char **argv){
	if (argc == 1 || activator(atoi(argv[1]),argv)){
		puts("USAGE : ./program [CODE] [ARGUMENTS...]\nCodes:\n0.Base64 Decode\n1.Base64 Encode\n2.To Hex\n3.From Hex\n4.Fixed XOR\n5.Input From A File\n6.Write To A File\n7.Bruteforce Single XOR\n\nYou can pipe output from one to another by using sequencing like 31 takes hex string and gives the base64 encoded string for it.");
	}
	return 0;
}