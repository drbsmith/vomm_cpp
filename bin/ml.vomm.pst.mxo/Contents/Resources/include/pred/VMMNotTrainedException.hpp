#ifndef _VMMNOTTRAINEDEX_HPP
#define _VMMNOTTRAINEDEX_HPP
/* HEADER
If you use this code don�t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04 

This code is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either version 2 
of the License, or (at your option) any later version. 

This code is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/ 
 
namespace vmm_pred {

/**
 * <p>Title: On prediction using VMM</p>
 * <p>Description:
 * Indicates that a VMM was not trained.
 * </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Technion</p>
 * @author Ron Begleiter
 * @version 1.0
 */

class VMMNotTrainedException : RuntimeException {
  public VMMNotTrainedException() {
  }

  public VMMNotTrainedException(String message) {
    super(message);
  }

  public VMMNotTrainedException(String message, Throwable cause) {
    super(message, cause);
  }

  public VMMNotTrainedException(Throwable cause) {
    super(cause);
  }
}

};

#endif // _VMMNOTTRAINEDEX_HPP
