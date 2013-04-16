%% Copyright © 2013, Université catholique de Louvain
%% All rights reserved.
%%
%% Redistribution and use in source and binary forms, with or without
%% modification, are permitted provided that the following conditions are met:
%%
%% *  Redistributions of source code must retain the above copyright notice,
%%    this list of conditions and the following disclaimer.
%% *  Redistributions in binary form must reproduce the above copyright notice,
%%    this list of conditions and the following disclaimer in the documentation
%%    and/or other materials provided with the distribution.
%%
%% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
%% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%% POSSIBILITY OF SUCH DAMAGE.

%%
%% Authors:
%%   Andrés F. Barco <anfelbar@javerianacali.edu.co>
%%   Alejandro Cardona <carealejo@gmail.com>
%%



functor

require
   BDB at 'x-oz://boot/BoolVar'
   

prepare
   
   FdRelType = '#'('=:':   'IRT_EQ'
		   '\\=:': 'IRT_NQ'
		   '<:':   'IRT_LE'
		   '<=:':  'IRT_LQ'
		   '>:':   'IRT_GR'
		   '>=:':  'IRT_LQ')
   
   FdConLevel = '#'(val: 'ICL_VAL'
		    bnd: 'ICL_BND'
		    dom: 'ICL_DOM')

   
   
export
%%% Finite domains
   decl:            BdDecl
   is:              BdIs
   value:           BdValue
   one:             BdOne
   zero:            BdZero
   none:            BdNone
      
define

   BdDecl = BDB.new
   BdIs = BDB.is
   BdValue = BDB.value
   BdOne = BDB.one
   BdZero = BDB.zero
   BdNone = BDB.none
   
end