/*
 * Copyright (C) 1999-2001  Haavard Kvaalen <havardk@xmms.org>
 *
 * Licensed under GNU LGPL version 2.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iconv.h>
#include <errno.h>

#ifdef HAVE_CODESET
#include <langinfo.h>
#endif

#include "charset.h"
#include "logging.h"

char* charset_get_current(void)
{
	char *charset = getenv("CHARSET");

#ifdef HAVE_CODESET
	if (!charset)
		charset = nl_langinfo(CODESET);
#endif
	if (!charset)
		charset = "ISO-8859-1";

	return charset;
}

char* charset_convert(const char *string, size_t insize, const char *from, const char *to)
{
	size_t outbytesleft, outbytes_initial, inbytesleft, outsize, nconv;
	iconv_t cd;
	char *out, *outptr;
	const char *input = string;
	size_t used = 0U;
	size_t null_char_width = 4U;
	int retry_no = 0;

	// sanity checks
	if (!string)
		return NULL;
	if(insize > (size_t)32768U)
		return NULL;

	if (!from)
		from = charset_get_current();
	if (!to)
		to = charset_get_current();


	if ((cd = iconv_open(to, from)) == (iconv_t)-1)
	{
		LOG(lm_main, LOG_ERROR, ("charset_convert(): iconv_open() = -1 => Conversion not supported. "
			  "Charsets: %s -> %s", from, to));
		goto Error_case_ret;
	}


	/* + 4 for nul in case len == 1 */
	/* For macOS Sonoma 14.4/15: makes more room because iconv behaves different than GNU and did not have errno == E2BIG !!! */

	outsize = (insize + 1) * null_char_width;	

	out = malloc(outsize);
	if(out == NULL)
	{
		LOG(lm_main, LOG_ERROR, ("Error in charset_convert(): malloc() ret NULL!"));		
      	goto Error_case_ret;
	}
	

	outbytesleft = outsize - null_char_width;
	outbytes_initial = outbytesleft;
	outptr = out;
	inbytesleft = insize;

//	LOG(lm_main, LOG_NOTICE, ("START charset_convert(%s -> %s): input=%p; inbytesleft=%zu, out=outptr=%p; outsize=%zu; outbytesleft=%zu",
//	from, to, input,inbytesleft,out,outsize,outbytesleft));


 retry:
#ifdef __lv2ppu__ 
 	if ((nconv=iconv(cd, (const char **) &input, &inbytesleft, &outptr, &outbytesleft)) == (size_t) -1)
#else
 	if ((nconv=iconv(cd,(char **) &input, &inbytesleft, &outptr, &outbytesleft)) == (size_t) -1)
#endif
	{

		switch (errno)
		{
			case E2BIG: /*(need more space - on macOS Sonoma 14.4/14.5 did not land here !!)*/
			
				// LOG(lm_main, LOG_NOTICE, ("charset_convert() -> case E2BIG; before realloc; input=%p; inbytesleft=%zu; outptr=%p; outbytesleft=%zu", 
				// input,inbytesleft,outptr, outbytesleft));

				used = 0;
				if(outptr - out > 0)
					used = (size_t)(outptr - out);
				else
				{
					if(outbytes_initial >= outbytesleft)
						used = (outbytes_initial - outbytesleft);
					else
						goto Error_case;
				}

				outsize = outsize * 2 + null_char_width;
				
				out = realloc(out, outsize);
				if(out == NULL)
				  goto Error_case;
				outptr = out + used;
				outbytesleft = outsize - used - null_char_width;
				outbytes_initial = outbytesleft;
				
				// LOG(lm_main, LOG_NOTICE, ("charset_convert() -> case E2BIG; after realloc; input=%p; out=%p; outptr=%p; outsize=%zu; used=%zu; outbytesleft=%zu", 
				// input,out,outptr,outsize,used, outbytesleft));

				if(retry_no < 10)
				{
					retry_no ++;
					goto retry;
				}

				else
					break;
			case EINVAL:
				LOG(lm_main, LOG_ERROR, ("charset_convert() -> case EINVAL; (Incomplete input sequence); "
			  		"input=%p; outsize=: %zu", input,outsize));
				goto Error_case;
				break;
			case EILSEQ: /*ill-formed input sequence*/
				/* Invalid sequence, try to get the rest of the string */
				LOG(lm_main, LOG_ERROR, ("charset_convert() -> case EILSEQ; (Invalid input sequence); "
			  		"input=%p; outptr=%p; outsize=%zu", input,outptr,outsize));								   
				if(input + 1 < string + insize)
				{
					input++;
					inbytesleft--;
					goto retry;
				}
				break;

			default:
				LOG(lm_main, LOG_ERROR, ("charset_convert() ->  case Default. Conversion failed. Charsets: %s -> %s; Inputstring: %s; "
					  "errno=%X;  Error: %s",
					  from,to,string,errno, strerror(errno)));
				goto Error_case;
		}
	}

	iconv(cd,NULL,NULL,&outptr,&outbytesleft);
	
    memset(outptr, 0, null_char_width);

	iconv_close(cd);

	// LOG(lm_main, LOG_NOTICE, ("END charset_convert() OK. nconv=%zu; input=%p; inbytesleft=%zu; out=%p; outptr=%p; outbytesleft=%zu; outbytes_initial=%zu",
	// 		  		nconv,input,inbytesleft,out, outptr,outbytesleft,outbytes_initial));

	return out;

Error_case:
	
	iconv_close(cd);
	if(out != NULL)free(out);

Error_case_ret:
	LOG(lm_main, LOG_ERROR, ("charset_convert() ->  Error_case"));
	
	out = malloc(insize);
	if(out != NULL)
	{
		//memset(out, 0, insize);    
		strncpy(out, string, insize);
	}

	return out;

	//return strdup(string);
}

char* charset_from_utf8(const char *string)
{
	if (!string)
		return NULL;
	return charset_convert(string, strlen(string), "UTF-8", NULL);
}

char* charset_to_utf8(const char *string)
{
	if (!string)
		return NULL;
	return charset_convert(string, strlen(string), NULL, "UTF-8");
}
