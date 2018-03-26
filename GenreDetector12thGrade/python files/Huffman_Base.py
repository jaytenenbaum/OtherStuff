def char_count(text):
    """ counts the number of occurances of ascii characters -
    ord(ch)<128, in a text. Returns a dictionary, with keys 
    being the observed characters, values being the counts """
    d = {}
    for ch in text:
        if ord(ch)>=128: continue
        if ch in d:
            d[ch] = 1+d[ch]
        else:
            d[ch]=1
    return d


######################################################################
#######  This is the "main" code, used for (de)compression ###########
#######  Huffman tree represented as list [left_son,right_son] #######

def build_huffman_list(letter_count):
    """ recieves dictionary with char:count entries
        generates a LIST of letters representing
        the binary Huffman encoding tree"""
    queue = sorted(letter_count.items(), key=lambda elem: elem[1])
    # sort by the count
    while len(queue) >= 2:
    # combine two smallest elements
        ax, ay = queue.pop(0)   # smallest in queue
        bx, by = queue.pop(0)   # next smallest
        chars = [ax,bx]
        weight = ay+by # combined weight
        queue.append((chars,weight)) # insert new pair at beginning of queue
        queue.sort(key=lambda elem: elem[1]) # re-arange (sort) queue
        
    return queue[0][0]  # peels off the irrelevant data (total weight)

def generate_code(huff_list, prefix=""):
    """ receives a Huffman list with embedded encoding,
        and a prefix of encodings.
        returns a dictionary where characters are
        keys and associated binary strings are values."""
    if isinstance(huff_list, str): # a leaf
        return {huff_list: prefix}
    else:
        lchild, rchild = huff_list[0], huff_list[1]
        codebook = {}

        codebook.update( generate_code(lchild, prefix+'0'))
        codebook.update( generate_code(rchild, prefix+'1'))
        #   oh, the beauty of recursion...
        return codebook
    
def compress(text, encoding_dict):
  ''' compress text using encoding dictionary '''
  assert isinstance(text, str)
  return ''.join(encoding_dict[ch] for ch in text if ord(ch)<=128)

def build_decoding_dict(encoding_dict):
   """build the "reverse" of encoding dictionary"""
   return {y:x for (x,y) in encoding_dict.items()}
  # return {y:x for x,y in encoding_dict.items()} # OK too
  
def decompress(bits, decoding_dict):
   prefix = ''
   result = []
   for bit in bits:
      prefix += bit
      if prefix not in decoding_dict:
         continue
      result.append(decoding_dict[prefix])
      prefix = ''
   assert prefix == '' # must finish last codeword
   return ''.join(result)  # converts list of chars to a string

######################################################################
#######  END code from lecture #######################################
######################################################################

def gen_full_asci():
    ''' Generate a string with all ascii chars '''
    asci = ""
    for c in range(128):
        asci+=chr(c)
    return asci
    #return str.join("",[chr(c) for c in range(128)])  #another way

def compression_ratio(text, corpus):
    d = generate_code(build_huffman_list(char_count(corpus+gen_full_asci())))
    len_compress = len(compress(text, d))
    len_unicode = len(text)*7
    return len_compress/len_unicode

  
