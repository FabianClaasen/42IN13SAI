#include "TokenizerController.h"
//#include <boost/date_time/posix_time/posix_time.hpp>

TokenizerController::TokenizerController(std::string filename)
{
	std::list<TokenDefinition> definitions = Grammar::getGrammar();
	std::vector<TokenPartner> partners = Grammar::getPartners();

	tokenizer = new Tokenizer(filename, definitions, partners);
}

void TokenizerController::Tokenize(std::streambuf* buffer)
{
	
    //boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time( );
	tokenizer->Tokenize(buffer);
    /*boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time( );
    
    boost::posix_time::time_duration elapsed = end - start;
    std::cout << elapsed << std::endl;*/
}

std::vector<std::shared_ptr<Token>> TokenizerController::GetCompilerTokens()
{
	return tokenizer->GetTokenList();
}

TokenizerController::~TokenizerController()
{
	delete tokenizer;
	tokenizer = nullptr;
}
