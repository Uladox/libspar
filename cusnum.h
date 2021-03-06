/* Include these
 * #include <stdlib.h> or <stddef.h>
 * #include "core.h"
 * #include "cases.h"
 * #include "text_utils.h"
 */

#define SPAR_BODY_CUSNUM_0(WHOLE_TYPE, WHOLE_SIZE,			\
			   DECIMAL_TYPE, DECIMAL_SIZE,			\
			   END_CASES, FAIL_LEN)				\
	int parsed = 1;							\
	char *curr = info->dat.text;					\
	int period = 0;							\
									\
	(void) parser;							\
									\
	token->dat.text = curr;						\
	token->type.generic = WHOLE_TYPE;				\
	token->data_size = WHOLE_SIZE;					\
									\
	switch (*curr) {						\
	SPAR_DIGIT_CASES:						\
		break;							\
	default:							\
		spar_text_error(info,					\
				"number does not start with digit", 0);	\
		token->len = FAIL_LEN;					\
		return 0;						\
	}								\
									\
	for (;; ++curr) {						\
		switch (*curr) {					\
		SPAR_DIGIT_CASES:					\
			continue;					\
		END_CASES:						\
		case '\0':						\
			goto out;					\
		case '.':						\
			token->type.generic = DECIMAL_TYPE;		\
			token->data_size = DECIMAL_SIZE;			\
			if (!period) {					\
				period = 1;				\
				break;					\
			}						\
			info->error.text =				\
				"too many decimal points in number";	\
		        goto error;					\
		default:						\
			info->error.text = "invalid character in number"; \
		error:							\
			info->cue.text->error_line = info->cue.text->lines;	\
			if (info->error_leave)				\
				return 0;				\
			parsed = 0;					\
			break;						\
		out:							\
			token->len = curr - token->dat.text;		\
			return parsed;					\
		}							\
	}

#define SPAR_BODY_CUSNUM(WHOLE_TYPE, WHOLE_SIZE,			\
			 DECIMAL_TYPE, DECIMAL_SIZE,			\
			 END_CASES, FAIL_LEN)				\
	SPAR_INSULATE(SPAR_BODY_CUSNUM_0(WHOLE_TYPE, WHOLE_SIZE,	\
					 DECIMAL_TYPE, DECIMAL_SIZE,	\
					 END_CASES, FAIL_LEN))
