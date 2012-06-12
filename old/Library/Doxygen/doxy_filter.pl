# Doxygen�p�t�B���^�����O�X�N���v�g
#
# Doxygen(Javadoc�x�[�X)�`���̕����s�R�����g����������A
# �s����Doxygen�̉��s�}�[�N(@n)������B
#
# �܂��A�����o�ϐ��Ƃ��}�N���Ƃ��̌���"///"�Ƃ����R�����g��
# ��������A"///<"�ɒu������B
#
# >perl doxy_filter.pl file_name
#
# Doxygen�ł́Aconf��INPUT_FILTER���ȉ��̗l�ɐݒ肷��B
# >INPUT_FILTER = "perl doxy_filter.pl"


$com = 0;	# �R�����g
$det = 0;	# "�ڍ�"�J�n
$at = 0;	# �O�̍s��@�ł���
$code = 0;	# @code �` @endcode���ł�

@buf;


$i = 0;
open(IN, $ARGV[0]);
while (<IN>) {
	if ($com == 0) {
		# �ʏ�̕���
		if ($_ =~ /^\s*\/\*[\*!].*/) {
			# �h�L�������e�[�V�����R�����g�J�n
			$com = 1;
			$det = 0;
			$at = 0;
			$code = 0;
		}
		# �����o�ϐ��̌���1�s�h�L�������e�[�V�����R�����g	
		elsif ($_ =~ /^\s*(class\s+|struct\s+|)(const\s+|)(unsigned\s+|signed\s+|)([\w\d_:\<\>]+|[\w\d_]+\s+[\w\d_]+)(\**|&)\s+[\w\d_]+;\s*\/\/[\/!][^<].*/) {
			$_ =~ s/(\/\/\/|\/\/!)/$1</;	# //!< �ɒu��������
		}
		# define�}�N���̌���1�s�h�L�������e�[�V�����R�����g
		elsif ($_ =~ /^\s*\#define\s+[\w\d_]+\s+[\w\d_\(\)]+\s*\/\/\/[^<].*/) {
			$_ =~ s/(\/\/\/|\/\/!)/$1</;
		}
		# typedef�̌���1�s�h�L�������e�[�V�����R�����g	
		elsif ($_ =~ /^\s*typedef\s+([\w\d_:\<\>]+|[\w\d_]+\s+[\w\d_]+)\**\s+[\w\d_]+;\s*\/\/\/[^<].*/) {
			$_ =~ s/(\/\/\/|\/\/!)/$1</;
		}
	} else {
		# �h�L�������e�[�V�����R�����g��
		if ($_ =~ /.*\*\/[\s\r\n]*$/) {
			# �h�L�������e�[�V�����R�����g�I��
			$com = 0;
		} else {
			if ($_ =~ /^(\s*|\s*\*)\s*\@.*/) {	# *�Ŏn�܂�Javadoc�ȃR�����g(\s*\*)�ƁA�󔒂�����Qt�̃R�����g(\s*)�����T�|�[�g
				# @�`�Ŏn�܂�^�O
				if ($_ =~ /.*\@code.*/) {
					# @code
					$code = 1;

					$buf[$i-1] =~ s/(�T���v��)/<b>$1<\/b>/;
						# �O�̍s��"�T���v��"�Ƃ�������ڗ������Ă݂�B
				}
				elsif ($_ =~ /.*\@endcode.*/) {
					# @endcode
					$code = 0;
				}
				else {
					$at = 1;
				}
			} else {
				if ($_ =~ /^(\s*|\s*\*)\s*$/) {
					# ��s
					$det = 1;	# �ЂƂ�s����������ȍ~�͏ڍׂƂ݂Ȃ�
				}
				elsif ($_ =~ /^(\s*|\s*\*)\s*.*$/) {
					# @�̕��͕����s�ł���
					if ($at == 1) {
						# @�̕����ɂ����s�t���܂��B
						$buf[$i-1] =~ s/(\n|\r\n)/\@n$1/;
					}
					
					if ($code == 0) {
						# ��
						if ($det == 1) {
							# �ڍׂȂ���ɉ��s�}�[�N��t����
							$_ =~ s/(\n|\r\n)/\@n$1/;
						}
					}
					elsif ($code == 1) {
						# �R�[�h�B�擪�̃^�u��1��菜���B
						$_ =~ s/(^\s*|^\s*\*)\t\t/$1\t/;
					}
				}
				$at = 0;
			}
		}
	}
	#print $_;
	$buf[$i] = $_;
	$i++;
}

foreach (@buf) {
	print $_;
}
close(IN);