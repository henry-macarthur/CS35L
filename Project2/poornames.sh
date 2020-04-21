#! /bin/sh
validateDirectory()
{
   
    if [ ! -d $1 ] 
    then
        1>&2 echo "input is not a directory"
        exit 1
    fi

if [ -z "$1" ]
then
    findBad '.'
elif [ $# -gt 1 ] 
then
    1>&2 echo "too many inputs"
    exit 1
elif [ ! -r "$1" ]
then
   1>&2 echo "no write access"
else
    findBad $1
fi


}



findBad()
{
    
    
    
    
    storAll="$(ls -a $1 | sort)" # contains all files
    dir="$(find $1 -maxdepth 1 -mindepth 1 -type d  -printf '%f\n' | sort)"
    nonASCII="$(ls -a  $1 | tr  ' ' '\n' |
 sed "/^[a-zA-Z\.\_\-]*$/d" | tr ' ' '\n')"
    nonASCII2="$( echo "$nonASCII" | tr ' ' '\n')" 
    uniqWords="$(ls -a $1 | uniq -i -d)"

    goodWords=""
    #echo "$uniqWords"
    IFS=$'\n'
    for i in ${uniqWords}
    do 
	#echo "$i"
	#echo "..."
	
	for k in ${storAll}
	do
	 #   echo "${k,,}"
	  #  echo "-----"
	    if [ "${i,,}" = "${k,,}" ]
	    then
		#echo "${i,,}"
		#testSring=$goodWords
		goodWords+="${k}"
		goodWords+=" "
		#goodWords+= " "
	    fi
	done
    done
    
    dp="$(echo "${goodWords}" | tr ' ' '\n')"
    dp="$(echo "${dp}"| sort -u | sed "/^ *$/d" )"
    
   
    
    
   
lng="$(find $1 -maxdepth 1 -mindepth 1 -name "???????????????*" -printf '%f\n')"
   
   
    startWithD="$(find $1 -maxdepth 1 -mindepth 1 | sed "/^[^\-]/d")" 
   
    
    #echo "${combinedFiles}"
    #echo "${duplicateWords}"
    newL="\n"
    dp=""${dp}" "${lng}" "${nonASCII2}" "${startWithD}""
    dp="$(echo "${dp}" | tr ' ' '\n')"
    #echo "${duplicateWords}"
    #get rid of .. and .
    cf2="$(echo "${dp}"  | sort -u |  sed "/^\.$/d" | sed "/^\.\.$/d")" 
    #echo "${combinedFiles2}"
    #echo "......."
    #echo "${dir}"
    #echo "......"
    IFS=$'\n'
    for i in ${dir}
    do
	for k in ${cf2}
	do
	   # echo "$k"
	    if [ $i = $k ]
	    then
		
		desired="${i}/"
		#echo "$desired"
	        cf2="$(echo "${cf2}" | sed "s|${i}|${desired}|g")"  
	    fi
	done
    done
   # echo "${cf2}"
    #echo "$combinedFiles2"
    f=""
    lastChar="${1: -1}"
    if [ "${lastChar}" = '/' ]
    then
    f="$(echo "${cf2}" | sed  "/^ *$/d" | sort -u  | sed 's|^|'"$1"'|g')"
    else
    f="$(echo "${cf2}" | sed  "/^ *$/d" | sort -u  | sed 's|^|'"$1"'\/|g')"
    fi

    #echo "${finalFiles}"
    IFS=$'\n'
    for i in ${f}
    do
	if [ "${i: -1}" != ' ' ]
	then
	    echo "$i"
	fi
    done
    DIR="$(find $1 -maxdepth 1 -mindepth 1 -type d)"
    
    DIRS="$(echo "${DIR}" | sed  "/^\.$/d" | sed  "/^\.\.$/d"| sed "/^ *$/d")"





    IFS=$'\n'
    #echo "$DIRS"
    if [ "$2" = '-r' ]
    then
	for k in ${DIRS}
	do
	    #echo $k
	    #echo $1
	    #echo "${dir}"
	    findBad $k '-r'
	done
    fi
    #rm storage
    #rm dirs
    #rm dr
     ##awk '{ print "DATA"$0}'  #'s/^/ /' | tr ' ' "$1"
#sort -u < storage
#for entry in "$1"/*
#do
#    echo "$entry"
#done
}



validateDirectory $1 $2

#rm storage
#rm dirs
#rm dr
