#include <iostream>
#include "C3dsParser.h"

C3dsParser::C3dsParser(string filename){
    std::cout << "parsing file " << filename << std::endl;
	m_currentHeader.length = NULL;
	m_currentHeader.chunkId = NULL;
	m_file = fopen(filename.c_str(), "rb");	
	// if (!m_file) TODO: log error
	fseek(m_file, 0, SEEK_END);
	m_fileLength = ftell(m_file);
	fseek(m_file, 0, SEEK_SET);
}

C3dsParser::~C3dsParser(){
	if ( m_file != NULL ) fclose(m_file);	
}

void  C3dsParser::enterChunk(){
	m_currentHeader.offset = ftell(m_file);
	fread(&m_currentHeader, 6, 1, m_file);
}

unsigned short C3dsParser::getChunkId(void){
	return m_currentHeader.chunkId;
}

unsigned int C3dsParser::getChunkLength(void){
	return m_currentHeader.length;
}

long int C3dsParser::getChunkOffset(void){
    return m_currentHeader.offset;
}

void C3dsParser::skipChunk(void){
	fseek(m_file, m_currentHeader.length + m_currentHeader.offset, SEEK_SET);
}

void C3dsParser::skipStrData(void){
	while ( fgetc(m_file) != '\0' ){}
}

unsigned int C3dsParser::getStrDataLength(void){
	long int tmp = ftell(m_file);
	skipStrData();
	unsigned int length = ftell(m_file) - tmp;
	fseek(m_file, tmp, SEEK_SET);
	return length; // includes null byte
}

char* C3dsParser::extractStrData(void){
	int len = getStrDataLength();
	char* str = new char[len];
	fread(str, sizeof(unsigned char), len, m_file);
	return str;
}

bool C3dsParser::eof(void){
	return ftell(m_file) >= m_fileLength;
}
