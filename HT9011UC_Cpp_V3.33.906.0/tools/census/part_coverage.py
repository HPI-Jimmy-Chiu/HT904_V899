#!/usr/bin/env python3
r"""Mechanical coverage oracle: what fraction of a golden line-range actually appears in a part?

WHY
"Present but ABBREVIATED" is the defect class census.py cannot see (a body with the right name
counts as done) and that audit agents describe in prose we then have to re-check by hand. This
measures it instead: for every SUBSTANTIVE golden line in the range, is a normalised form of that
line present in the part file?

It is deliberately blunt and deliberately conservative about what counts as a miss:
  * Comment-only and blank golden lines are ignored -- their transcription matters, but that is a
    different check, and counting them would drown the signal.
  * Normalisation strips ALL whitespace and trailing ';', because indentation legitimately changes
    when a body is re-nested, and because a translated line may gain a trailing comment.
  * A golden line that appears ANYWHERE in the part counts as present, not necessarily in order.
    So this can pass on a scrambled body -- it is a coverage measure, not an order check. Brace
    delta and the seam checks are what cover order.
  * Lines that legitimately CHANGE in translation (VCL -> shim calls, AnsiString ctors) will show
    as misses. So a non-zero miss list is normal; what matters is the RATE and whether the misses
    are explainable. Read them, do not just look at the percentage.

USAGE
    python part_coverage.py <goldenFile> <partsDir> <name:start:end> [...]
    python part_coverage.py --spec w7c        # built-in spec for the PT-W7c parts
"""
import io, os, re, sys

GOLD_ROOT = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618'
PORT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0'

W7C = ('aTester_Front.cpp', PORT + '/_w7c_parts', [
    ('00257_TestZ1OutRandge2.txt', 257, 289),
    ('02227_InitFTestSuckTestICTask.txt', 2227, 2230),
    ('02237_DoFTestSuckTestIC.txt', 2237, 3941),
    ('03945_DoFRTCUseSocketFloat.txt', 3945, 4069),
    ('04073_DoFRTCAutoModelVerify.txt', 4073, 4918),
    ('04922_DoIndexArm2PickUpErrNeedPiggyback.txt', 4922, 5078),
    ('05080_CheckAnyCaseNeedToDoArm1.txt', 5080, 5152),
    ('05155_InitTestYFrontTask.txt', 5155, 5158),
    ('05165_DoTestYFront_c1.txt', 5165, 6118),
    ('06119_DoTestYFront_c2.txt', 6119, 7035),
    ('07036_DoTestYFront_c3.txt', 7036, 8007),
    ('08011_InitFrontTestPurgBeforePickShuttle.txt', 8011, 8014),
    ('08016_DoFrontTestPurgBeforePickShuttle.txt', 8016, 8096),
    ('08098_FTestNeedDestroy.txt', 8098, 8108),
    ('08110_FTestSeparateSLK.txt', 8110, 8199),
    ('08201_FTestCombineSLK.txt', 8201, 8437),
    ('08439_DoArm1Suck.txt', 8439, 8540),
    ('08542_DoArm1D44VacCheck.txt', 8542, 8579),
])


def norm(s):
    s = s.split('//')[0]
    s = re.sub(r'/\*.*?\*/', '', s)
    s = re.sub(r'\s+', '', s)
    return s.rstrip(';')


def substantive(line):
    t = line.strip()
    if not t or t.startswith('//') or t.startswith('/*') or t.startswith('*'):
        return False
    if norm(line) == '':
        return False
    return True


def main():
    if '--spec' in sys.argv and 'w7c' in sys.argv:
        gname, pdir, spec = W7C
    else:
        if len(sys.argv) < 4:
            print(__doc__)
            return 2
        gname, pdir = sys.argv[1], sys.argv[2]
        spec = []
        for a in sys.argv[3:]:
            n, s, e = a.rsplit(':', 2)
            spec.append((n, int(s), int(e)))

    gpath = os.path.join(GOLD_ROOT, gname)
    raw = io.open(gpath, 'rb').read()
    eol = '\r\n' if raw.count(b'\r\n') else '\n'
    GL = raw.decode('cp950', 'replace').split(eol)

    print('golden: %s (%d lines)' % (gname, len(GL)))
    print('%-46s %7s %7s %7s  %s' % ('part', 'golden', 'present', 'cover', 'worst misses'))
    tot_g = tot_p = 0
    allmiss = []
    for name, a, b in spec:
        p = os.path.join(pdir, name)
        if not os.path.exists(p):
            print('%-46s %7s %7s %7s  (part not on disk)' % (name, b - a + 1, '-', '-'))
            continue
        praw = io.open(p, 'rb').read()
        ptxt = praw.decode('utf-8', 'replace')
        pset = set(norm(l) for l in ptxt.replace('\r\n', '\n').split('\n'))
        pset.discard('')
        want = [(a + i, GL[a - 1 + i]) for i in range(b - a + 1) if substantive(GL[a - 1 + i])]
        miss = [(ln, t) for ln, t in want if norm(t) not in pset]
        got = len(want) - len(miss)
        tot_g += len(want)
        tot_p += got
        pct = (100.0 * got / len(want)) if want else 100.0
        print('%-46s %7d %7d %6.1f%%  %s'
              % (name, len(want), got, pct,
                 ('%d missing' % len(miss)) if miss else 'none'))
        allmiss.extend((name, ln, t) for ln, t in miss[:6])
    print('\nTOTAL substantive golden lines %d, present %d -> %.2f%%'
          % (tot_g, tot_p, 100.0 * tot_p / tot_g if tot_g else 100.0))
    if allmiss:
        print('\n--- sample misses (READ THESE; translation legitimately changes some lines) ---')
        for name, ln, t in allmiss[:40]:
            print('  %-40s golden:%-6d %s' % (name[:40], ln, t.strip()[:78]))
    return 0


if __name__ == '__main__':
    sys.exit(main())
