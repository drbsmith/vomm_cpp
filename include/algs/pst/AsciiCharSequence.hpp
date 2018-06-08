/* HEADER 
If you use this code don’t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04 

This code is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either version 2 
of the License, or (at your option) any later version. 

This code is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/ 

namespace vmm {
//package vmm.algs.pst;

//import java.nio.ByteBuffer;

/**
 * <p>Title: Probabilistic Suffix Tree</p>
 * <p>Description: Initializing the PST algorithm</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: Foretell</p>
 * @author Ron Begleiter
 * @version 1.0
 */

    class AsciiCharSequence : public CharSequence {
    private:
        const int UNSIGNED_BYTE_MASK = 0xFF;

        StringBuffer sbuff;
        int start;
        int end;

  private AsciiCharSequence(StringBuffer sb, int starts, int ends){
    sbuff = sb;
    start = starts;
    end = ends;
  }

  public AsciiCharSequence(ByteBuffer bbuff) {
    byte []bytes = bbuff.array();
    sbuff = new StringBuffer(new String(bytes));
    start = 0;
    end = sbuff.length();
  }

    public int length(){
        return end-start;
    }

  public char charAt(int index){
    if(index<length())
      return sbuff.charAt(start+index);
    else
      throw new IndexOutOfBoundsException("ILL index at chatAt");
  }


  public CharSequence subSequence(int starts, int ends){
    if(start*end<0 || end<start || end>length())
      throw new IndexOutOfBoundsException("(start*end<0 || end<start || end>length())");
    return new AsciiCharSequence(sbuff,starts,ends);
  }


  public String toString(){
    return sbuff.substring(start,end);
  }

  public static void main(String []args){
    byte []b={49,49,49,49,50,50,50,50};
    CharSequence a = new AsciiCharSequence(ByteBuffer.wrap(b));
    for(int i=a.length(); i>-1; --i){
      a = a.subSequence(0,i);
      System.out.println(">"+a);
    }
  }
};
    
}
