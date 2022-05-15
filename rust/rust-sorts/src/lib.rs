fn bubble_sort(seq : &mut Vec<i32>)
{
    for unsorted_bound in (1..seq.len()+1).rev()
    {
        for i in 0..(unsorted_bound - 1)
        {
            if seq[i] > seq[i + 1]
            {
                let tmp = seq[i + 1];
                seq[i + 1] = seq[i];
                seq[i] = tmp;
            }
        }
    }
}

static SORTS_TO_TEST: &'static [fn(&mut Vec<i32>)] = &[bubble_sort];

#[cfg(test)]
mod tests
{
    use super::SORTS_TO_TEST;

    #[test]
    fn sort_1_element()
    {
        let expected_output = vec![1];
        for sort_func in SORTS_TO_TEST
        {
            let mut a = vec![1];
            sort_func(&mut a);
            assert_eq!(a, expected_output);
        }
    }

    #[test]
    fn sort_n_element()
    {
        let expected_output = vec![1, 2, 4, 6, 8];
        for sort_func in SORTS_TO_TEST
        {
            let mut a = vec![4, 1, 8, 6, 2];
            sort_func(&mut a);
            assert_eq!(a, expected_output);
        }
    }
}
