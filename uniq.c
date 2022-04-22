int rep;
int df = 0, uf = 0;

wchar_t *acLine(wchar_t *buf, size_t buflen, FILE *fp) {
	size_t bufpos;
	wint_t ch;
	bufpos = 0;
	while (bufpos + 2 != buflen && (ch = getwc(fp)) != WEOF && ch != '\n') {
		buf[bufpos++] = ch;
	}
	if (bufpos + 1 != buflen) {
		buf[bufpos] = '\0';
	}
	while (ch != WEOF && ch != '\n') {
		ch = getwc(fp);
	}
	return (bufpos != 0 || ch == '\n' ? buf : NULL);
}

void printing(FILE *ofp, wchar_t *str) {
	if ((df && rep) || (uf && !rep)) {
		fprintf(ofp, "%ls\n", str);
	}
}

FILE *file(const char *name, const char *mode) {
	FILE *fp;
	if ((fp = fopen(name, mode)) == NULL) {
		err(1, "%s", name);
	}
	return(fp);
}

int scomp(wchar_t *s1, wchar_t *s2) {
	wchar_t *p, line1[MAX_LINE], line2[MAX_LINE];
	for (p = line1; *s1; s1++) {
		*p++ = towlower(*s1);
	}
	*p = '\0';
	for (p = line2; *s2; s2++) {
		*p++ = towlower(*s2);
	}
	*p = '\0';
	return (wcscoll(line1, line2));
}

void uniq (int argc, char *argv[]) {
    argc = argc - 1;
	wchar_t *t1, *t2;
	FILE *ifp;
	FILE *ofp;
	int ch;
	wchar_t *prevline, *thisline;
	char *p;
	int ifl = 0, comp;
	int ok = 1;

	if((ch = getopt(argc, argv, "cdif:s:u")) != -1) {
		if(strcmp(argv[1],"-i")==0) { 
			ifl = 1;
		}
		else if (strcmp(argv[1],"-d")==0) {
			df = 1;
		}
		else if (strcmp(argv[1],"-u")==0) {
			uf = 1;
		}
		else {
			ok = 0;
		}
	}

	argc -= optind;
	argv += optind;
	
    if (!df && !uf) {
		df = 1;
		uf = 1;
	}

    if (argc > 2) {
        printf("uniq: unrecognized option\n");
	}

	FILE* stdinold = stdin;
	FILE* stdoutold = stdout;

	ifp = stdin;
	ofp = stdout;

	if (argc > 0 && argv[0] && strcmp(argv[0], "-") != 0) {
		ifp = file(argv[0], "r");
	}
	if (argc > 1 && argv[1]) {
		ofp = file(argv[1], "w");
	} 

	prevline = malloc(MAX_LINE * sizeof(*prevline));
	thisline = malloc(MAX_LINE * sizeof(*thisline));

	if (prevline == NULL || thisline == NULL) {
		err(1, "malloc");
	}

	if (acLine(prevline, MAX_LINE, ifp) == NULL) {
		if (ferror(ifp)) {
			err(1, "%s", ifp == stdin ? "stdin" : argv[0]);
		}
	}

	if (uf && df && ok == 1) {
		printing(ofp, prevline);
	}

	while (acLine(thisline, MAX_LINE, ifp)) {
		t1 = thisline;
		t2 = prevline;

		if (ifl)
			comp = scomp(t1, t2);
		else
			comp = wcscoll(t1, t2);

		if (comp) {
			if (!df || !uf && ok == 1) {
				printing(ofp, prevline);
			}
			t1 = prevline;
			prevline = thisline;
			if (uf && df && ok == 1) {
				printing(ofp, prevline);
			}
			thisline = t1;
			rep = 0;
		} else {
			++rep;
		}
	}
	if (ferror(ifp)) {
		err(1, "%s", ifp == stdin ? "stdin" : argv[0]);
	}
	if ((!df || !uf) && ok == 1) {
		printing(ofp, prevline);
	}
	fclose(ifp);
	fclose(ofp);
	ifp = stdinold;
	ofp = stdoutold;
	exit(0);
}