/*
 ********************************************************************
 * The RPG Toolkit Version 3 Plugin Libraries
 * This file copyright (C) 2003-2007  Christopher B. Matthews
 ********************************************************************
 *
 * This file is released under the AC Open License v 1.0
 * See "AC Open License.txt" for details
 ********************************************************************
 * Copyright (C) 2013 Lorie Jay C. Gutierrez
 * piartsco@gmail.com
 ********************************************************************
 */

/*
 * Includes
 */

#include "stdafx.h"
#include "sdk\tkplugin.h"
#include <string>
#include "rpgcode.h"


/*********************************************************************/
///////////////////////////////////////////////////////
// RPGCODE INTERFACE
//
// If this is an RPGCode plugin, you must modify
// TKPlugQuery
// TKPlugExecute
///////////////////////////////////////////////////////

void getParameters(std::string text, PARAMETERS &toRet)
{

    //************************************************************************************
    // Gets parameters of a command
    //************************************************************************************
	PARAM_DETAILS p;
	std::string brackets;

    brackets = CBGetBrackets(text);

    long countData;
    countData = CBCountBracketElements(brackets);
    toRet.dataCount = countData;

	//toRet.paras = new PARAM_DETAILS[toRet.dataCount];

    long elementIdx;
	for (elementIdx = 0; elementIdx < countData; elementIdx++)
	{
		//toRet.paras[elementIdx] = new PARAM_DETAILS();
		std::string theElem;
        theElem = CBGetBracketElement(brackets, elementIdx + 1);

        RPGC_DT theDT;
		theDT = (RPGC_DT)CBGetElementType(theElem);

		p.dat = theElem;
		p.dataType = theDT;

		toRet.paras.push_back(p);

        switch(theDT)
		{
		case DT_NUM:
			//Numerical element!
            toRet.paras[elementIdx].num = CBGetNumElementValue(theElem);
			break;
		case DT_LIT:
            //Literal element!
            toRet.paras[elementIdx].lit = CBGetStringElementValue(theElem);
			break;
		}
	}
}

void tk3Defaults(std::string commandLine, TYPE_RETVAL *retVal)
{
	// Parse parameters
    PARAMETERS paras;
	getParameters(commandLine, paras);

 //   if(paras.dataCount != 3)
	//{
	//	CBMessageWindow("tk3Defaults requires 3 Parameters!");
	//	return;
	//}
	//if (paras.paras[0].dataType != DT_NUM)
	//{
	//	CBMessageWindow("tk3Defaults argument 1 must be NUMERIC!");
	//	return;
	//}
	//if (paras.paras[1].dataType != DT_NUM)
	//{
	//	CBMessageWindow("tk3Defaults argument 2 must be NUMERIC!");
	//	return;
	//}
	
	CBMessageWindow("tk3 Default Systems");
    retVal->dataType = DT_NUM;
    retVal->num = 0;
}

///////////////////////////////////////////////////////
//
// Function: TKPlugQuery
//
// Parameters: pstrQuery - string passed in.
//
// Action: passes in an rpgcode command name.
//				this function is supposed to indicate
//				if it does or does not support this command
//
// Returns: 1 (if we can support the command)
//					0 (if we cannot support the command)
//
///////////////////////////////////////////////////////
int APIENTRY TKPlugQuery(char *pstrQuery)
{
	//TBD: Add query code here.
	std::string strQuery(pstrQuery);
	for (int ctr = 0; ctr < strQuery.length(); ctr++)
	{
		strQuery[ctr] = tolower(strQuery[ctr]);
	}

	if (strQuery == "tk3defaultsinfo")
		return 1;
	return 0;
}


///////////////////////////////////////////////////////
//
// Function: TKPlugExecute
//
// Parameters: pstrCommand - command to execute
//
// Action: passes in an rpgcode command.
//				it is up to the plugin to execute
//				this command.
//
// Returns: 1 (command executed OK)
//					0 (error executing command)
//
///////////////////////////////////////////////////////
int APIENTRY TKPlugExecute(char *pstrCommand, int &retValDt, char *retValLit, double &retValNum, const short usingReturn)
{
	//TBD: Add execution code here.
	TYPE_RETVAL retVal;
    retVal.usingReturnData = (bool)usingReturn;

	if (CBGetCommandName(pstrCommand) == "tk3defaultsinfo")
		tk3Defaults(std::string(pstrCommand), &retVal);

    retValDt = (int)retVal.dataType;
	int len = retVal.lit.length() + 1;
	memcpy(retValLit, retVal.lit.c_str(), len);
    retValNum = retVal.num;

	return 1;
}


