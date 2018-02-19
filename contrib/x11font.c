/* Copyright (C) 2002 Gerd Knorr */

/* $Id: x11font.c,v 1.5 2006/02/10 06:25:36 mschimek Exp $ */

#include "src/exp-gfx.c"

static void print_head(FILE *fp,
		       const char *foundry,
		       const char *name,
		       const char *slant,
		       int width, int height)
{
    fprintf(fp,
	    "STARTFONT 2.1\n"
	    "FONT -%s-%s-Medium-%s-Normal--%d-%d-75-75-C-%d-ISO10646-1\n"
	    "SIZE %d 75 75\n"
	    "FONTBOUNDINGBOX %d %d 0 0\n"
	    "STARTPROPERTIES 25\n"
	    "FONTNAME_REGISTRY \"\"\n"
	    "FOUNDRY \"%s\"\n"
	    "FAMILY_NAME \"%s\"\n"
	    "WEIGHT_NAME \"Medium\"\n"
	    "SLANT \"%s\"\n"
	    "SETWIDTH_NAME \"Normal\"\n"
	    "ADD_STYLE_NAME \"\"\n"
	    "PIXEL_SIZE %d\n"
	    "POINT_SIZE %d\n"
	    "RESOLUTION_X 75\n"
	    "RESOLUTION_Y 75\n"
	    "SPACING \"C\"\n"
	    "AVERAGE_WIDTH %d\n"
	    "CHARSET_REGISTRY \"ISO10646\"\n"
	    "CHARSET_ENCODING \"1\"\n"
	    "COPYRIGHT \"fixme\"\n"
	    "CAP_HEIGHT 9\n"
	    "X_HEIGHT 18\n"
	    "FONT \"-%s-%s-Medium-%s-Normal--%d-%d-75-75-C-%d-ISO10646-1\"\n"
	    "WEIGHT 10\n"
	    "RESOLUTION 75\n"
	    "QUAD_WIDTH %d\n"
	    "DEFAULT_CHAR 0\n"
	    "FONT_ASCENT %d\n"
	    "FONT_DESCENT 0\n"
	    "ENDPROPERTIES\n",
	    foundry,name,slant,height,height*10,width*10,
	    height,
	    width,height,
	    foundry,name,slant,height,height*10,width*10,
	    foundry,name,slant,height,height*10,width*10,
	    width,height);
}

static void
print_font(const char *filename,
	   const char *foundry,
	   const char *name, int italic,
	   uint8_t *font, int cw, int ch, int cpl, int hs,
	   int count, unsigned int (*map)(unsigned int,int), int invalid)
{
    FILE *fp;
    int x,y,s,i,c,on,bit,byte,mask1,mask2;

    fp = stdout;
    if (NULL != filename) {
	fp = fopen(filename,"w");
	if (NULL == fp)
	    fprintf(stderr,"open %s: %s\n",filename,strerror(errno));
	fprintf(stderr,"writing %s\n",filename);
    }
    
    print_head(fp, foundry, name, italic ? "I" : "R", cw, ch*hs);
    fprintf(fp,"CHARS %d\n", count);
    
    for (i = 0; i < 0xffff; i++) {
	c = map(i, italic);
	if (invalid == c)
	    continue;

	fprintf(fp,"STARTCHAR fixme\n"
		"ENCODING %d\n"
		"SWIDTH %d 0\n"
		"DWIDTH %d 0\n"
		"BBX %d %d 0 0\n"
		"BITMAP\n",
		i,cw*10,cw,
		cw,ch*hs);
	for (y = 0; y < ch; y++) {
	    for (s = 0; s < hs; s++) {
	    bit  = cpl * cw * y + cw * c;
	    byte = 0;
	    for (x = 0; x < cw; x++) {
		mask1 = 1 << (bit & 7);
		mask2 = 1 << (7-(x & 7));
		on    = font[bit >> 3] & mask1;
		if (on)
		    byte |= mask2;
		if (7 == (x&7)) {
		    fprintf(fp,"%02x",byte);
		    byte = 0;
		}
		bit++;
	    }
	    fprintf(fp,"%02x\n",byte);
	}
	}
	fprintf(fp,"ENDCHAR\n");
    }
    fprintf(fp,"ENDFONT\n");

    if (NULL != filename)
	fclose(fp);
}

int 
main(int argc, char *argv[])
{
    print_font("teletext1.bdf","Ets","Teletext",0,wstfont2_bits,
	       TCW,TCH,TCPL,1,1448,unicode_wstfont2,357);
    print_font("teletext2.bdf","Ets","Teletext",0,wstfont2_bits,
	       TCW,TCH,TCPL,2,1448,unicode_wstfont2,357);
    print_font("teletext4.bdf","Ets","Teletext",0,wstfont2_bits,
	       TCW,TCH,TCPL,4,1448,unicode_wstfont2,357);
    print_font("teletext1i.bdf","Ets","Teletext",1,wstfont2_bits,
	       TCW,TCH,TCPL,1,1449,unicode_wstfont2,357);
    print_font("teletext2i.bdf","Ets","Teletext",1,wstfont2_bits,
	       TCW,TCH,TCPL,2,1449,unicode_wstfont2,357);
    print_font("teletext4i.bdf","Ets","Teletext",1,wstfont2_bits,
	       TCW,TCH,TCPL,4,1449,unicode_wstfont2,357);
    print_font("caption.bdf","Ets","Caption",0,ccfont2_bits,
	       CCW,CCH,CCPL,1,120,unicode_ccfont2,15);
    print_font("captioni.bdf","Ets","Caption",1,ccfont2_bits,
	       CCW,CCH,CCPL,1,120,unicode_ccfont2,15 + 4 * 32);
    return 0;
}
