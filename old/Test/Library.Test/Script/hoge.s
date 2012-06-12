set $a, "hage"
set $b, 2

goto HOGE

echo $a
echo $b

if $a == "hage"
	echo "a:true."
	if $b == 2
HOGE:
		echo "b:true"
	end
end

echo "moke"

if $a == "hage"
	echo "a:true2"
end
