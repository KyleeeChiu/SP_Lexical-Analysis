#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std ;

struct Table {
	string name ;
	bool exist ;
};

struct Line { 
	int line ;
	int type ;
	int value ;
};

class Hash {
public:
	void make( Table * & table ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table[i].name = "" ;
			table[i].exist = false ;
		}
	}
	
	int getHash( string name ) {
		int result = 0 ;
		for ( int i = 0 ; i < strlen( name.c_str() ) ; i++ ) {
			int plus = (int)name.c_str()[i] ;
			result = result + plus ;
		}
		
		return result % 100 ;
	}
	
	int getTable( string name, Table * & table ) {
		int value = getHash( name ) ;
		return findOrInsert( value, name, table ) ;
	}
	
	int findOrInsert( int value, string name, Table * & table ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( table[value].exist == false ) {
				table[value].exist = true ;
				table[value].name = name ;
				return value ;
			}
			else if ( name == table[value].name ) {
				return value ;
			}
			else {
				value = ( value + 1 ) % 100 ;
				i++ ;
			}
		}
		
		return -1 ;
	}
};

class AllTable {
	Hash hash ;
	vector <string> table1 ;
	vector <string> table2 ;
	vector <string> table3 ;
	vector <string> table4 ;	
	Table * table5 ;
	Table * table6 ;
	Table * table7 ;
public:		
	AllTable() {
		table5 = new Table[100] ;
		table6 = new Table[100] ;
		table7 = new Table[100] ;
	}
	
	void MakeTable( string num, vector <string> & table ) {
		string str = "Table" + num + ".table" ;
		fstream file ;
		int stNo = 0 ;
		file.open( str.c_str(), fstream::in );
		string a, final = "" ;
		while ( file >> a ) {
			table.push_back( a ) ;
		}
		file.close();		
	}
	
	void makeHash() {
		hash.make( table5 );
		hash.make( table6 );
		hash.make( table7 );
	}
	
	void make() {
		MakeTable( "1", table1 );
		MakeTable( "2", table2 );
		MakeTable( "3", table3 );
		MakeTable( "4", table4 );
		makeHash() ;
	}
	
	int getHashValue( string token, int table ) {
		if ( table == 5 )
			return hash.getTable( token, table5 ) ;
		else if ( table == 6 )
			return hash.getTable( token, table6 ) ;
		else if ( table == 7 )
			return hash.getTable( token, table7 ) ;		
	}
	
	int insSize() {
		return table1.size() ;
	}
	
	int exSize() {
		return table2.size() ;
	}
	
	int regSize() {
		return table3.size() ;
	}
	
	int delSize() {
		return table4.size() ;
	}
	
	string insToken( int pos ) {
		return table1.at( pos ) ;
	} 
	
	string exToken( int pos ) {
		return table2.at( pos ) ;
	} 
	
	string regToken( int pos ) {
		return table3.at( pos ) ;
	} 
	
	string delToken( int pos ) {
		return table4.at( pos ) ;
	} 
};

vector <string> input ;

class Token {
	string token ;
	Hash hash ;
	AllTable table ;
	vector <Line> output ;
public:
	Token( AllTable table1 ) {
		table = table1 ;
	}
	
	bool IsWhiteSpace( char ch ) {
		if ( ch ==' ' || ch == '\t' || ch == '\n' )
			return true ;
		return false ;	
	}
	
	bool IsInstruction( string token, int & value ) {
		for ( int i = 0 ; i < table.insSize() ; i++ ) {
			if ( stricmp( token.c_str(), table.insToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;				
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsPseudo( string token, int & value ) {
		for ( int i = 0 ; i < table.exSize() ; i++ ) {
			if ( stricmp( token.c_str(), table.exToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;	
			}
		}
		
		value = 0 ;
		return false ;
	}
	bool IsRegister( string token, int & value ) {
		for ( int i = 0 ; i < table.regSize() ; i++ )	{
			if ( stricmp( token.c_str(), table.regToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;	
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsDelimiter( string token, int & value ) {
		for ( int i = 0 ; i < table.delSize() ;i++ ) {
			if ( strcmp( token.c_str(), table.delToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsDelimiter( char ch ) {
		for ( int i = 0 ; i < table.delSize() ; i++ ) {
			if ( table.delToken( i ).c_str()[0] == ch )
				return true ;
		}
		return false ;
	}
	
	bool IsInteger( string token ) {
		if ( token[0] < '0' || token[0] > '9' ) 
			return false ;
		return true ;
	}
	
	void getValue( string token, int & type, int & value, int special ) {
		if ( special == 1 ) {
			type = 7 ;
			value = table.getHashValue( token, 7 ) ;
		}
		else if ( special == 2 ) {
			type = 6 ;
			string change ;
			transform(token.begin(), token.end(), back_inserter(change), ::toupper);
			value = table.getHashValue( change, 6 ) ;
		}
		else if ( IsInstruction( token, value ) ) {
			type = 1 ;
		}
		else if ( IsPseudo( token, value ) ) {
			type = 2 ;
		}
		else if ( IsRegister( token, value ) ) {
			type = 3 ;
		} 
		else if ( IsDelimiter( token, value ) ) {
			type = 4 ;
		}
		else if ( IsInteger( token ) ) {
			type = 6 ;
			value = table.getHashValue( token, 6 ) ;
		}
		else { 
			type = 5 ;
			value = table.getHashValue( token, 5 ) ;
		}
	}

	void addResult( int line, int type, int value ) {
		Line temp ;
		temp.line = line ;
		temp.type = type ;
		temp.value = value ;
		output.push_back( temp );
	}
	
	void getToken() {
		bool skip = false, notToken = false ;
		int special = -1, quote = 0 ;
		int type, value ;
		for ( int i = 0 ; i < input.size() ; i++ ) {
			for ( int j = 0 ; j < input.at( i ).size() ; j++ ) {
				if ( input[i][j] == '\n' ) {
					if ( special != -1 && quote == 1 )
						token = "" ;
					quote = 0 ;
					special = -1 ;					
				}
				if ( skip ) {
					if ( input[i][j] == '\n' ) 
						skip = false ;
					token = "" ;	
				}
				else if ( IsWhiteSpace( input[i][j] ) ) {
					if ( special != -1 ) {
						token = token + input[i][j] ;
					}
					else {
						if ( token != "" ) {
							getValue( token, type, value, -1 ) ;
							addResult( i, type, value ) ;
						}
						token = "" ;
					}
				}
				else if ( IsDelimiter( input[i][j] ) ) {
					if ( special != -1 && input[i][j] != '\'' && quote == 1 ) 
						token = token + input[i][j] ;
					else {
					if ( input[i][j] == '\'' ) {
						quote++ ;
						if ( stricmp( token.c_str(), "C" ) == 0 ) {
							notToken = true ;
							special = 1 ;
						}	
						else if ( stricmp( token.c_str(), "X" ) == 0 ) {
							notToken = true ;
							special = 2 ;
						}
					}
					else if ( input[i][j] == '.' ) {
						skip = true ;
					}
					
					if ( !notToken && token != "" ) {
						getValue( token, type, value, special ) ;
						addResult( i, type, value ) ;					
					}
					notToken = false ;	
					token = input[i][j] ;
					if ( quote == 2 )
						special = -1 ;
					
					getValue( token, type, value, -1 ) ;
					addResult( i, type, value ) ;
					token = "" ;
					
					}
				}
				else {
					token = token + input[i][j] ;
				}
			}
		}
	}
	
	void print() {
		cout << endl ;
		int j = 0 ;
		for ( int i = 0 ; i < input.size() ; i++ ) {
			cout << input.at( i ) ;
			for ( ;j < output.size() ; j++ ) {
				if ( output.at( j ).line != i ) {
					if ( input.at( i ) != "\n" )
						cout << endl ;
					break ;	
				}
				else {
					cout << "(" << output.at( j ).type << "," << output.at( j ).value << ")" ;
				}	
			}
		}
		
		cout << endl ;
	}
	
	void outFile( string fileName ) {
		fstream outFile ;
		outFile.open( fileName.c_str(), fstream::out );
		int j = 0 ;
		for ( int i = 0 ; i < input.size() ; i++ ) {
			outFile << input.at( i ) ;
			for ( ;j < output.size() ; j++ ) {
				if ( output.at( j ).line != i ) {
					if ( input.at( i ) != "\n" )
						outFile << endl ;
					break ;	
				}
				else {
					outFile << "(" << output.at( j ).type << "," << output.at( j ).value << ")" ;
				}	
			}
		}
		
		outFile << endl ;
		outFile.close();
	}
	
	void clear() {
		output.clear() ;
	}
};

bool readFile( string fileName ) {
	fstream file ;
	file.open( fileName.c_str(), fstream::in ) ;
	if ( !file.is_open() ) {
		cout << endl << "### The input file is not on the directory! ###" << endl ;
		return false ;
	}
	else {
		string temp ;
		while ( getline( file, temp ) ) {
			temp = temp + '\n' ;
			input.push_back( temp ) ;
		}
		
		file.close();
		return true ;
	}	
}

int main() {
	string command ;
	AllTable table ;
	table.make() ;
	Token token( table ) ; 
	do {
		string fileName ;
		cout << endl << "***Assembler Implement****" ;
		cout << endl << "* 0. Exit                *" ;
		cout << endl << "* 1. Lexical Analysis    *" ;
		cout << endl << "**************************" ;
		cout << endl << "Input a command(0, 1): " ;
		cin >> command ;
		if ( command.compare( "1" ) == 0 ) {
			cout << endl << "Input file name: " ;
			cin >> fileName ;
			if ( readFile( fileName ) ) {
				token.getToken() ;	
				token.print() ;			
				cout << endl << "Output file name([0]Quit): " ;
				cin >> fileName ;
				if ( fileName != "0" )
					token.outFile( fileName ) ;
			}
			token.clear() ;
			input.clear() ;
			table.makeHash() ;			
		}
		else if ( command.compare( "0" ) == 0 )	
			break ;
		else
			cout << endl << "### Command does not exist! ###" << endl ;			
	} while( true ) ;
	system("pause") ;
} 
