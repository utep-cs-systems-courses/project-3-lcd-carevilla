.data
jt:	jumptable
	.word_case0
	.word_case1
	.word_case2
	.word_case3
	.word_default

swNum:	.word

.text
	
	mov &r12 , &swNum

jt:	
	cmp #0, &swNum
	jz case_0
	cmp #1 , &swNum
	jz case_1
	cmp #2 , &swNum
	jz case_2
	cmp #3 , swNum
	jz case_3
	cmp #4 , &swNum
	jnc default
	
case1:
	call #drawBar
	goto out	 /* if carry flag on go to exit while */

case2:
	call #drawBar
	goto out

case3:
	call #drawBar
	goto out

default:
	goto out

out:
	mov &swNum , &r12
	pop &r12

	
