void Pen::DrawWuLine( CFbsBitmap & screen, int X0, int Y0, int X1, int Y1, TRgb clrLine, TInt penSize)
{
  /* Make sure the line runs top to bottom */
  if (Y0 > Y1)
    {
      int Temp = Y0; Y0 = Y1; Y1 = Temp;
      Temp = X0; X0 = X1; X1 = Temp;
    }
    
  /* Draw the initial pixel, which is always exactly intersected by
     the line and so needs no weighting */
  DrawPixel(screen, X0, Y0, clrLine );
    
  int XDir, DeltaX = X1 - X0;
  if( DeltaX >= 0 )
    {
      XDir = 1;
    }
  else
    {
      XDir   = -1;
      DeltaX = 0 - DeltaX; /* make DeltaX positive */
    }
    
  /* Special-case horizontal, vertical, and diagonal lines, which
     require no weighting because they go right through the center of
     every pixel */

  int DeltaY = Y1 - Y0;
  if (DeltaY == 0)
    {
      /* Horizontal line */
      while (DeltaX-- != 0)
        {
	  X0 += XDir;
	  DrawPixel(screen, X0, Y0, clrLine );

	  for(int w = 1; w < penSize - 1; w++)
	    DrawPixel(screen, X0, Y0+w, clrLine);
        }
      return;
    }
  if (DeltaX == 0)
    {
      /* Vertical line */
      do
        {
	  Y0++;
	  DrawPixel(screen, X0, Y0, clrLine );
	  
	  for(int w = 1; w < penSize - 1; w++)
	    DrawPixel(screen, X0, Y0+w, clrLine);

        } while (--DeltaY != 0);
      return;
    }
  
  if (DeltaX == DeltaY)
    {
      /* Diagonal line */
      do
	{
	  X0 += XDir;
	  Y0++;
	  DrawPixel(screen, X0, Y0, clrLine );
	  
	  for(int w = 1; w < penSize - 1; w++)
	    DrawPixel(screen, X0, Y0+w, clrLine);
	  
	} while (--DeltaY != 0);
      return;
    }
  
  unsigned short ErrorAdj;
  unsigned short ErrorAccTemp, Weighting;
    
  /* Line is not horizontal, diagonal, or vertical */
  unsigned short ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
    
  TInt rl = (clrLine.Value() & 0xff0000) >> 16;
  TInt gl = (clrLine.Value() & 0x00ff00) >> 8;
  TInt bl = (clrLine.Value() & 0xff);

  double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;
    
  /* Is this an X-major or Y-major line? */
  if (DeltaY > DeltaX)
    {
      /* Y-major line; calculate 16-bit fixed-point fractional part of a
	 pixel that X advances each time Y advances 1 pixel, truncating the
	 result so that we won't overrun the endpoint along the X axis */

      ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;

      /* Draw all pixels other than the first and last */
      while (--DeltaY) {
	ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
	ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
	if (ErrorAcc <= ErrorAccTemp) {
	  /* The error accumulator turned over, so advance the X coord */
	  X0 += XDir;
	}
	Y0++; /* Y-major, so always advance Y */
	/* The IntensityBits most significant bits of ErrorAcc give us the
	   intensity weighting for this pixel, and the complement of the
	   weighting for the paired pixel */
	Weighting = ErrorAcc >> 8;
	ASSERT( Weighting < 256 );
	ASSERT( ( Weighting ^ 255 ) < 256 );
	
	
	//get the color
	screen.LockHeap();
	TUint32 * tmp = screen.DataAddress();
	TUint32 clrBackground = tmp[X0+Y0*240];
	screen.UnlockHeap();


	TInt rb = (clrBackground & 0xff0000) >> 16;
	TInt gb = (clrBackground & 0x00ff00) >> 8;
	TInt bb = (clrBackground & 0xff);

	double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;


	double rr = ( rb > rl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							      (Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) :
		      ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
				      / 255.0 * ( rl - rb ) + rb ) ) );
	double gr = ( gb > gl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							      (Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) :
		      ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
				      / 255.0 * ( gl - gb ) + gb ) ) );
	double br = ( bb > bl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							      (Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) :
		      ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
				      / 255.0 * ( bl - bb ) + bb ) ) );

	DrawPixel(screen, X0, Y0, TRgb(rr,gr,br));


	for(int w = 1;w < penSize-1; w++)
	  {
	    DrawPixel(screen, X0, Y0+w, clrLine);
	  }
           

	//get the color
	screen.LockHeap();
	tmp = screen.DataAddress();
	clrBackground = tmp[X0+XDir+(Y0+penSize)*240];
	screen.UnlockHeap();

	rb = (clrBackground & 0xff0000) >> 16;
	gb = (clrBackground & 0x00ff00) >> 8;
	bb = (clrBackground & 0xff);

	grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

	rr = ( rb > rl ? (/* ( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						       Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) :
	       ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
			       / 255.0 * ( rl - rb ) + rb ) ) );
	gr = ( gb > gl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						       Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) :
	       ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
			       / 255.0 * ( gl - gb ) + gb ) ) );
	br = ( bb > bl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						       Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) :
	       ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
			       / 255.0 * ( bl - bb ) + bb ) ) );
	
	DrawPixel(screen, X0 + XDir, Y0+penSize, TRgb(rr,gr,br));
      }

      /* Draw the final pixel, which is always exactly intersected by the line
	 and so needs no weighting */
      DrawPixel(screen, X1, Y1, clrLine);
      return;
    }

  //else if(deltax > deltay)
  /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
     pixel that Y advances each time X advances 1 pixel, truncating the
     result to avoid overrunning the endpoint along the X axis */
  ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
  /* Draw all pixels other than the first and last */
  while (--DeltaX) {
    ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
    ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
    if (ErrorAcc <= ErrorAccTemp) {
      /* The error accumulator turned over, so advance the Y coord */
      Y0++;
    }
    X0 += XDir; /* X-major, so always advance X */
    /* The IntensityBits most significant bits of ErrorAcc give us the
       intensity weighting for this pixel, and the complement of the
       weighting for the paired pixel */
    Weighting = ErrorAcc >> 8;
    ASSERT( Weighting < 256 );
    ASSERT( ( Weighting ^ 255 ) < 256 );

    //get the color
    screen.LockHeap();
    TUint32 * tmp = screen.DataAddress();
    TUint32 clrBackground = tmp[X0+Y0*240];
    screen.UnlockHeap();

    TInt rb = (clrBackground & 0xff0000) >> 16;
    TInt gb = (clrBackground & 0x00ff00) >> 8;
    TInt bb = (clrBackground & 0xff);
    
    double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

    double rr = ( rb > rl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							  (Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) : 
		  ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) 
				  / 255.0 * ( rl - rb ) + rb ) ) );
    double gr = ( gb > gl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							  (Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) : 
		  ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) 
				  / 255.0 * ( gl - gb ) + gb ) ) );
    double br = ( bb > bl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:
							  (Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) : 
		  ( /*( TInt )*/( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) 
				  / 255.0 * ( bl - bb ) + bb ) ) );

    DrawPixel(screen, X0, Y0, TRgb(rr, gr, br));

    for(int w = 1;w < penSize-1; w++)
      {
	DrawPixel(screen, X0, Y0+w, clrLine);
      }
 
    screen.LockHeap();
    tmp = screen.DataAddress();
    //clrBackground = tmp[X0+(Y0+1)*240];
    clrBackground = tmp[(X0)+(Y0+penSize)*240];
    screen.UnlockHeap();


    rb = (clrBackground & 0xff0000) >> 16;
    gb = (clrBackground & 0x00ff00) >> 8;
    bb = (clrBackground & 0xff);
    grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

    rr = ( rb > rl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						   Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) : 
	   ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) 
			   / 255.0 * ( rl - rb ) + rb ) ) );
    gr = ( gb > gl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						   Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) : 
	   ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) 
			   / 255.0 * ( gl - gb ) + gb ) ) );
    br = ( bb > bl ? ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):
						   Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) : 
	   ( /*( TInt )*/( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) 
			   / 255.0 * ( bl - bb ) + bb ) ) );

    //DrawPixel(screen, X0, Y0 + 1, TRgb(rr, gr, br));
    DrawPixel(screen, X0, Y0 + (penSize), TRgb(rr, gr, br));
    

    for(int w = 1;w < penSize-1; w++)
      {
	DrawPixel(screen, X0, Y0+w, TRgb(0,0,0));
      }

  }
    
  /* Draw the final pixel, which is always exactly intersected by the line
     and so needs no weighting */
  DrawPixel(screen, X1, Y1, clrLine);
}
